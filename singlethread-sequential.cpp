#include <iostream>
#include "transpose.h"

using namespace std;

Matrix transpose_sequential(const Matrix& A, bool verbose) {
    if (verbose) printf("[VERBOSE] Starting sequential matrix transpose.\n");
    Matrix B(A[0].size(), vector<int>(A.size())); //make the x by y matrix
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[0].size(); j++) {
            B[j][i] = A[i][j];
        }
    }
    if (verbose) printf("[VERBOSE] Tranpose complete.\n");
    return B;
}

int main() {
    Matrix A = transpose_buildMatrix(1000, 1000, true);
    Matrix B = transpose_time(transpose_sequential, A, true, "Singlethread Sequential");
    return 0;
}