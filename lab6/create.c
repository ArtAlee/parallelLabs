#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char **argv) {
    int array_size = atoi(argv[1]);
    FILE *file = fopen("array.txt", "w");
	
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }


    srand(time(NULL));

    for (int i = 0; i < array_size; ++i) {
        fprintf(file, "%d ", rand()%10000); 
    }

    fclose(file);

    return 0;
}
