#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_VAL 1000

void combine(int data[], int buffer[], int l, int m, int r) {
    int x = l, y = m, z = l;

    while (x < m && y < r) {
        if (data[x] <= data[y])
            buffer[z++] = data[x++];
        else
            buffer[z++] = data[y++];
    }

    while (x < m) buffer[z++] = data[x++];
    while (y < r) buffer[z++] = data[y++];

    for (x = l; x < r; x++)
        data[x] = buffer[x];
}

int main() {

    int main_arr[SIZE_VAL], aux_arr[SIZE_VAL];

    for (int idx = 0; idx < SIZE_VAL; idx++)
        main_arr[idx] = rand() % 1000;

    clock_t t1 = clock();

    for (int step = 1; step < SIZE_VAL; step *= 2) {
        for (int pos = 0; pos < SIZE_VAL; pos += 2 * step) {

            int l = pos;
            int m = (pos + step < SIZE_VAL) ? pos + step : SIZE_VAL;
            int r = (pos + 2 * step < SIZE_VAL) ? pos + 2 * step : SIZE_VAL;

            combine(main_arr, aux_arr, l, m, r);
        }
    }

    clock_t t2 = clock();

    double elapsed_ms = ((double)(t2 - t1)) / CLOCKS_PER_SEC * 1000;

    printf("CPU Pipelined Time = %f ms\n", elapsed_ms);

    return 0;
}
