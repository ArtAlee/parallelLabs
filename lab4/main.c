#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <locale.h>
#define SIZE 10000000

double find_max(int* arr, int numthreads) {
    double begin = omp_get_wtime();
    int mx = -1;
#pragma omp parallel num_threads(numthreads) shared(arr) reduction(max:mx)
    {
//#pragma omp for schedule ( static )
#pragma omp for schedule ( static,5)
//#pragma omp for schedule ( static, 5)
//#pragma omp for schedule ( dynamic )
//#pragma omp for schedule ( dynamic ,5)
//#pragma omp for schedule ( guided)
//#pragma omp for schedule ( guided,5)
//#pragma omp for schedule ( auto)
    for (int i = 0; i < SIZE; ++i) {
        if (arr[i] > mx) {
            mx = arr[i];
        }
    }

    }

    double end = omp_get_wtime();
    return end-begin;
}


int main() {
    int* arr = (int*)malloc(SIZE * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = rand() % 1000;
    }

    FILE* fp = fopen("result.txt", "w");
    setlocale(LC_NUMERIC, "");

    // Запускаем функцию для разного числа потоков
    for (int numthreads = 1; numthreads <= 8; ++numthreads) {
        double avgTime =0;
        for (int i = 0; i < 10; ++i) {
            avgTime += find_max(arr, numthreads);
        }
        avgTime /= 10;
        fprintf(fp, "%f\n", avgTime);
    }
    fclose(fp);
    free(arr);

    return 0;
}