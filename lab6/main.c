#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (rank == 0)
        if ((size & (size - 1)) != 0) {
            fprintf(stderr, "number of threads must be a power of 2.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_TOPOLOGY);
        }


    int array_size = atoi(argv[1]);
    if (rank == 0)
        if (array_size % size != 0) {
            fprintf(stderr, "size of the array must be a multiple of the number of threads.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_SIZE);
        }

    int *array;
    if (rank == 0) {
        array = (int *) malloc(array_size * sizeof(int));
        if (!array) {
            fprintf(stderr, "Error! Failed to allocate memory.\n");
            MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
        }
    }

    if (rank == 0) {
        FILE *fd = fopen("array.txt", "r");
        for (int i = 0; i < array_size; i++)
            fscanf(fd, "%d", &array[i]);
        fclose(fd);

    }
    int chunk_size = array_size / size;
    int *chunk = (int *) malloc(chunk_size * sizeof(int));
    int *tmp_chunk = (int *) malloc(chunk_size * sizeof(int));
    int *new_chunk = (int *) malloc(chunk_size * sizeof(int));
    if (!chunk || !tmp_chunk || !new_chunk) {
        fprintf(stderr, "Error! Failed to allocate memory.\n");
        MPI_Abort(MPI_COMM_WORLD, MPI_ERR_NO_MEM);
    }
    double start_time, end_time;
    if (rank == 0)
        start_time = MPI_Wtime();

    MPI_Scatter(array, chunk_size, MPI_INTEGER, chunk, chunk_size, MPI_INTEGER, 0, MPI_COMM_WORLD);

    for (int d = chunk_size / 2; d > 0; d /= 2)
        for (int i = d; i < chunk_size; i++)
            for (int j = i - d; j >= 0; j -= d)
                if (chunk[j] > chunk[j + d]) {
                    int tmp = chunk[j];
                    chunk[j] = chunk[j + d];
                    chunk[j + d] = tmp;
                }

    int power = (int)log2(size);
    for (int n = 0; n < power; n++) {

        int pair = rank ^ (1 << (power - n - 1));

        MPI_Sendrecv(chunk, chunk_size, MPI_INTEGER, pair, n, tmp_chunk, chunk_size, MPI_INTEGER, pair, n,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (rank < pair && chunk[chunk_size - 1] > tmp_chunk[0]) {
            int i = 0;
            int j = 0;
            int k = 0;
            while (k < chunk_size)
                if (chunk[i] < tmp_chunk[j])
                    new_chunk[k++] = chunk[i++];
                else
                    new_chunk[k++] = tmp_chunk[j++];

            memcpy(chunk, new_chunk, chunk_size * sizeof(int));

        } else if (rank > pair && chunk[0] < tmp_chunk[chunk_size - 1]) {
            int i = chunk_size - 1,
                    j = chunk_size - 1,
                    k = chunk_size - 1;

            while (k >= 0)
                if (chunk[i] < tmp_chunk[j])
                    new_chunk[k--] = tmp_chunk[j--];

                else
                    new_chunk[k--] = chunk[i--];

            memcpy(chunk, new_chunk, chunk_size * sizeof(int));

        }
    }

    MPI_Gather(chunk, chunk_size, MPI_INTEGER, array, chunk_size, MPI_INTEGER, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        end_time = MPI_Wtime();

        FILE *fd = fopen("result.txt", "a+t");
        fprintf(fd, "%lf \n", end_time - start_time);
        fclose(fd);
    }

    free(chunk);
    free(tmp_chunk);
    free(new_chunk);
    if (!rank)
        free(array);

    MPI_Finalize();

    return 0;
}
