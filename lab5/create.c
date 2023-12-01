#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 10000000

int main() {
    FILE *file = fopen("array.txt", "w");

    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }


    srand(time(NULL));

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        fprintf(file, "%d ", rand() % 10000);
    }

    fclose(file);

    return 0;
}
