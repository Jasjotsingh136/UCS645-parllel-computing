#include <mpi.h>
#include <stdio.h>

#define N 100

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array[N];
    int local_n = N / size;
    int local_array[local_n];


    if (rank == 0) {
        for (int i = 0; i < N; i++)
            array[i] = i + 1;
    }


    MPI_Scatter(array, local_n, MPI_INT,
                local_array, local_n, MPI_INT,
                0, MPI_COMM_WORLD);


    int local_sum = 0;
    for (int i = 0; i < local_n; i++)
        local_sum += local_array[i];


    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        printf("Global Sum = %d\n", global_sum);
        printf("Average = %.2f\n", global_sum / (float)N);
    }

    MPI_Finalize();
    return 0;
}
