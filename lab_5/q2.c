#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000

void bcast_manual(double *a, int n, int rt, MPI_Comm c) {
    int r, p;
    MPI_Comm_rank(c, &r);
    MPI_Comm_size(c, &p);

    if (r == rt) {
        for (int i = 0; i < p; i++) {
            if (i != rt)
                MPI_Send(a, n, MPI_DOUBLE, i, 0, c);
        }
    } else {
        MPI_Recv(a, n, MPI_DOUBLE, rt, 0, c, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int r, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    double *a = malloc((size_t)N * sizeof(double));
    if (!a) {
        if (r == 0) printf("ERR: malloc failed\n");
        MPI_Finalize();
        return 0;
    }

    // --- Manual Broadcast ---
    for (int i = 0; i < N; ++i)
        a[i] = (r == 0) ? 1.2345 : 0.0;

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    bcast_manual(a, N, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t2 = MPI_Wtime();
    double dt1 = t2 - t1;

    // --- MPI_Bcast ---
    for (int i = 0; i < N; ++i)
        a[i] = (r == 0) ? 9.8765 : 0.0;

    MPI_Barrier(MPI_COMM_WORLD);
    double t3 = MPI_Wtime();

    MPI_Bcast(a, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t4 = MPI_Wtime();
    double dt2 = t4 - t3;

    double m1 = 0.0, m2 = 0.0;
    MPI_Reduce(&dt1, &m1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&dt2, &m2, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (r == 0) {
        printf("p=%d manual=%lf mpi=%lf\n", p, m1, m2);
    }

    free(a);
    MPI_Finalize();
    return 0;
}
