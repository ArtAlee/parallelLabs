#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


struct ArrayElement {
    int count;
    int value;
    omp_lock_t lock;
};

#define ARRAY_SIZE 1000

int main() {
    struct ArrayElement array[ARRAY_SIZE];


    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i].count = 0;
        array[i].value = 0;
        omp_init_lock(&array[i].lock);
    }

#pragma omp parallel num_threads(2)
    {

        for (int i = 0; i < 100000; ++i) {
            int index = rand() % ARRAY_SIZE;
            omp_set_lock(&array[index].lock);
            array[index].count++;
            omp_unset_lock(&array[index].lock);
        }
    }

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        omp_destroy_lock(&array[i].lock);
    }

    return 0;
}
