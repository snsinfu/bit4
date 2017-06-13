#include <stdlib.h>

struct {
    int rows;
    int cols;
    double *data;
//  double(* data)[rows][cols];     // ERROR
} typedef Matrix;

Matrix make_matrix(int rows, int cols) {
    return (Matrix) {
        .rows = rows,
        .cols = cols,
        .data = malloc(sizeof(double[rows][cols]))
    };
}

void randomize_matrix(Matrix matrix) {
    int rows = matrix.rows;
    int cols = matrix.cols;
    double(*data)[rows][cols] = (double(*)[rows][cols]) matrix.data;

    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            (*data)[i][j] = (double) rand() / RAND_MAX;
        }
    }
}

int main(void) {
    Matrix matrix = make_matrix(12345, 67890);
    randomize_matrix(matrix);
    free(matrix.data);
}
