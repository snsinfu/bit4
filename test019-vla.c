#include <stdio.h>
#include <stdlib.h>

void foo(int rows, int cols, double matrix[rows][cols]) {
    // These lines do not change the VLA dimensions.
    rows--;
    cols--;
    printf("%g\n", matrix[23][45]);
}

void run(int rows, int cols) {
    double(* matrix)[rows][cols] = malloc(sizeof *matrix);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            (*matrix)[i][j] = i + j;
        }
    }
    foo(rows, cols, *matrix);

    free(matrix);
}

int main(void) {
    run(123, 456);
}
