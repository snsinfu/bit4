// Mallocing VLA.

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void randomize_matrix(int rows, int cols, double matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (double) rand() / RAND_MAX;
        }
    }
}

void run(int rows, int cols) {
    double(* matrix)[rows][cols] = malloc(sizeof *matrix);
    randomize_matrix(rows, cols, *matrix);
    free(matrix);
}

int main(void) {
    run(12345, 67890);
}
