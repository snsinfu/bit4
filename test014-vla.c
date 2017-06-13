#include <stdio.h>

void print_matrix(int rows, int cols, double mat[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.3g%c", mat[i][j], "\t\n"[j + 1 == cols]);
        }
    }
}

int main(void) {
    double mat[3][4] = {
        {1.0, 2.0, 3.0, 4.0},
        {5.0, 6.0, 7.0, 8.0},
        {9.0, 0.0, 1.0, 2.0},
    };
    print_matrix(3, 4, mat);
}
