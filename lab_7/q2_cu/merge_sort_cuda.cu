#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

#define SIZE_N 1000

__global__ void mergeStep(int *data_in, int *data_tmp, int seg_width, int total_n) {
    int th_id = blockIdx.x * blockDim.x + threadIdx.x;
    int left = th_id * 2 * seg_width;

    if (left >= total_n) return;

    int middle = min(left + seg_width, total_n);
    int right = min(left + 2 * seg_width, total_n);

    int p = left, q = middle, r = left;

    while (p < middle && q < right) {
        if (data_in[p] < data_in[q])
            data_tmp[r++] = data_in[p++];
        else
            data_tmp[r++] = data_in[q++];
    }

    while (p < middle) data_tmp[r++] = data_in[p++];
    while (q < right) data_tmp[r++] = data_in[q++];
}

int main() {
    int host_arr[SIZE_N];

    for (int i = 0; i < SIZE_N; i++)
        host_arr[i] = rand() % 1000;

    int *gpu_arr, *gpu_buf;

    cudaMalloc(&gpu_arr, SIZE_N * sizeof(int));
    cudaMalloc(&gpu_buf, SIZE_N * sizeof(int));

    cudaMemcpy(gpu_arr, host_arr, SIZE_N * sizeof(int), cudaMemcpyHostToDevice);

    cudaEvent_t t_start, t_stop;
    cudaEventCreate(&t_start);
    cudaEventCreate(&t_stop);

    cudaEventRecord(t_start);

    int blk_size = 256;

    for (int curr_width = 1; curr_width < SIZE_N; curr_width *= 2) {
        int thread_count = (SIZE_N + (2 * curr_width - 1)) / (2 * curr_width);
        int grid_count = (thread_count + blk_size - 1) / blk_size;

        mergeStep<<<grid_count, blk_size>>>(gpu_arr, gpu_buf, curr_width, SIZE_N);

        int *swap_ptr = gpu_arr;
        gpu_arr = gpu_buf;
        gpu_buf = swap_ptr;
    }

    cudaEventRecord(t_stop);
    cudaEventSynchronize(t_stop);

    float exec_time;
    cudaEventElapsedTime(&exec_time, t_start, t_stop);

    printf("CUDA Merge Sort Time = %f ms\n", exec_time);

    cudaFree(gpu_arr);
    cudaFree(gpu_buf);

    return 0;
}
