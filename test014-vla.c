#include <stdio.h>
#include <stdlib.h>

void print_matrix(int rows, int cols, double mat[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.3f%c", mat[i][j], "\t\n"[j + 1 == cols]);
        }
    }
}

void print_random_matrix(int rows, int cols) {
    double mat[rows][cols];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i][j] = (double) rand() / RAND_MAX;
        }
    }
    print_matrix(rows, cols, mat);
}

int main(void) {
    print_random_matrix(3, 4);
}
