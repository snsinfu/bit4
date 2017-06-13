void normalize(int n, double distr[n]) { // Still equivalent to double* distr
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += distr[i];
    }
    for (int i = 0; i < n; i++) {
        distr[i] /= sum;
    }
}

int main(void) {
    double distr[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double* p_distr = distr;
    normalize(5, p_distr); // Okay
}
