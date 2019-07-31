#include <stdio.h>
#include <omp.h>


int main()
{
    int n = 10000;
    double array[10000] = {0};

#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        array[i] += 10;
    }
    return 0;
}
