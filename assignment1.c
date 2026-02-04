#include <stdio.h>
#include <omp.h>

int main() {
    int size = 150000;
    double arrA[size], arrB[size];
    double factor = 2.5;
    int idx, threadCount;
    double startTime, endTime;
    double serialDuration;

    for (idx = 0; idx < size; idx++) {
        arrA[idx] = 1.0;
        arrB[idx] = 2.0;
    }

    for (idx = 0; idx < size; idx++)
        arrA[idx] = 1.0;

    startTime = omp_get_wtime();
    for (idx = 0; idx < size; idx++) {
        arrA[idx] = factor * arrA[idx] + arrB[idx];
    }
    endTime = omp_get_wtime();

    serialDuration = endTime - startTime;
    printf("Serial Time = %f seconds\n", serialDuration);

    for (threadCount = 2; threadCount <= 12; threadCount++) {
        double parallelDuration, acceleration;

        omp_set_num_threads(threadCount);

        for (idx = 0; idx < size; idx++)
            arrA[idx] = 1.0;

        startTime = omp_get_wtime();
        #pragma omp parallel for schedule(static)
        for (idx = 0; idx < size; idx++) {
            arrA[idx] = factor * arrA[idx] + arrB[idx];
        }
        endTime = omp_get_wtime();

        parallelDuration = endTime - startTime;
        acceleration = serialDuration / parallelDuration;

        printf("Threads = %d  Time = %f  Speedup = %.2f\n", 
               threadCount, parallelDuration, acceleration);
    }

    printf("arrA[0] = %f\n", arrA[0]);

    return 0;
}

