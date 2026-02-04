
//Question 1




/* #include <stdio.h>
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

*/






















//Question 2 




/*
#include <stdio.h>
#include <omp.h>

double matX[1000][1000], matY[1000][1000], matZ[1000][1000];

int main() {

    int size = 1000;
    int row, col, depth, Threads;
    double start, end;
    double serial_time;

    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            matX[row][col] = 1.0;
            matY[row][col] = 2.0;
            matZ[row][col] = 0.0;
        }
    }

    printf("Sequential:\n");
    start = omp_get_wtime();
    for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++) {
            for (depth = 0; depth < size; depth++) {
                matZ[row][col] += matX[row][depth] * matY[depth][col];
            }
        }
    }
    end = omp_get_wtime();
    serial_time = end - start;
    printf("Time = %f\n", serial_time);

    printf("\n1D Parallel:\n");
    for (Threads = 2; Threads <= 10; Threads++) {

        double parallel_time, speedup;
        omp_set_num_threads(Threads);

        for (row = 0; row < size; row++)
            for (col = 0; col < size; col++)
                matZ[row][col] = 0.0;

        start = omp_get_wtime();
        #pragma omp parallel for private(col, depth)
        for (row = 0; row < size; row++) {
            for (col = 0; col < size; col++) {
                for (depth = 0; depth < size; depth++) {
                    matZ[row][col] += matX[row][depth] * matY[depth][col];
                }
            }
        }
        end = omp_get_wtime();

        parallel_time = end - start;
        speedup = serial_time / parallel_time;

        printf("Threads=%d  Time=%f  Speedup=%.2f\n",
               Threads, parallel_time, speedup);
    }

    printf("\n2D Parallel:\n");
    for (Threads = 2; Threads <= 10; Threads++) {

        double parallel_time, speedup;
        omp_set_num_threads(Threads);

        for (row = 0; row < size; row++)
            for (col = 0; col < size; col++)
                matZ[row][col] = 0.0;

        start = omp_get_wtime();
        #pragma omp parallel for collapse(2) private(depth)
        for (row = 0; row < size; row++) {
            for (col = 0; col < size; col++) {
                for (depth = 0; depth < size; depth++) {
                    matZ[row][col] += matX[row][depth] * matY[depth][col];
                }
            }
        }
        end = omp_get_wtime();

        parallel_time = end - start;
        speedup = serial_time / parallel_time;

        printf("Threads=%d  Time=%f  Speedup=%.2f\n",
               Threads, parallel_time, speedup);
    }

    return 0;
}

*/






// Question 3







#include <stdio.h>
#include <omp.h>

int main() {

    long totalSteps = 1000000;
    double step;
    int i, Threads;
    double xPos, sumValue;
    double startTime, endTime, seqTime;
    double pi;

    step = 1.0 / (double) totalSteps;

    /* Sequential */
    sumValue = 0.0;
    startTime = omp_get_wtime();

    for (i = 0; i < totalSteps; i++) {
        xPos = (i + 0.5) * step;
        sumValue = sumValue + 4.0 / (1.0 + xPos * xPos);
    }

    pi = step * sumValue;
    endTime = omp_get_wtime();
    seqTime = endTime - startTime;

    printf("Sequential:\n");
    printf("Pi = %.10f\n", pi);
    printf("Time = %f\n", seqTime);

    /* Parallel */
    printf("\nParallel:\n");
    for (Threads = 2; Threads <= 10; Threads++) {

        double parTime, speedup;

        omp_set_num_threads(Threads);
        sumValue = 0.0;

        startTime = omp_get_wtime();
        #pragma omp parallel for private(xPos) reduction(+:sumValue)
        for (i = 0; i < totalSteps; i++) {
            xPos = (i + 0.5) * step;
            sumValue = sumValue + 4.0 / (1.0 + xPos * xPos);
        }

        pi = step * sumValue;
        endTime = omp_get_wtime();

        parTime = endTime - startTime;
        speedup = seqTime / parTime;

        printf("Threads=%d  Pi=%.10f  Time=%f  Speedup=%.2f\n",
               Threads, pi, parTime, speedup);
    }

    return 0;
}

