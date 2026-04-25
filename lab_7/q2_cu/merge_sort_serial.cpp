#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_VAL 1000

void mergeParts(int data[], int left, int mid, int right) {
    int p = left, q = mid + 1, idx = 0;
    int buffer[right - left + 1];

    while (p <= mid && q <= right) {
        if (data[p] <= data[q])
            buffer[idx++] = data[p++];
        else
            buffer[idx++] = data[q++];
    }

    while (p <= mid) buffer[idx++] = data[p++];
    while (q <= right) buffer[idx++] = data[q++];

    for (p = left, idx = 0; p <= right; p++, idx++)
        data[p] = buffer[idx];
}

void divideSort(int data[], int start, int end) {
    if (start < end) {
        int center = (start + end) / 2;
        divideSort(data, start, center);
        divideSort(data, center + 1, end);
        mergeParts(data, start, center, end);
    }
}

int main() {
    int arr_main[SIZE_VAL];

    for (int i = 0; i < SIZE_VAL; i++)
        arr_main[i] = rand() % 1000;

    clock_t t_begin = clock();

    divideSort(arr_main, 0, SIZE_VAL - 1);

    clock_t t_finish = clock();

    double duration = ((double)(t_finish - t_begin)) / CLOCKS_PER_SEC * 1000;

    printf("Serial Merge Sort Time = %f ms\n", duration);

    return 0;
}
