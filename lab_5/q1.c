#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N (1<<16)

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int r, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    double a = 2.5;

    if (N % p != 0) {
        if (r == 0)
            printf("Error: N=%d not divisible by p=%d\n", N, p);
        MPI_Finalize();
        return 0;
    }

    int n = N / p;

    double *x = NULL, *y = NULL;

    if (r == 0) {
        x = malloc(N * sizeof(double));
        y = malloc(N * sizeof(double));

        for (int i = 0; i < N; ++i) {
            x[i] = 1.0 + 0.01 * (i % 100);
            y[i] = 2.0 + 0.02 * (i % 50);
        }
    }

    double *lx = malloc(n * sizeof(double));
    double *ly = malloc(n * sizeof(double));

    MPI_Scatter(x, n, MPI_DOUBLE, lx, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(y, n, MPI_DOUBLE, ly, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    for (int i = 0; i < n; ++i)
        lx[i] = a * lx[i] + ly[i];

    MPI_Barrier(MPI_COMM_WORLD);
    double t2 = MPI_Wtime();

    double dt = t2 - t1, tmax;

    MPI_Reduce(&dt, &tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Gather(lx, n, MPI_DOUBLE, x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (r == 0) {
        printf("p=%d time=%lf\n", p, tmax);
        free(x);
        free(y);
    }

    free(lx);
    free(ly);

    MPI_Finalize();
    return 0;
}
