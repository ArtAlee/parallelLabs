#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

bool is_prime(int num) {
    if (num < 2) {
        return false;
    }
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);



    int primes_count = 0;
    int *primes = malloc((end - start + 1) * sizeof(int));



    clock_t start_time = clock();

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            primes[primes_count++] = i;
        }
    }

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time: %lf\n", execution_time);

    free(primes);

    return 0;
}
