#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
using Matrix = vector<vector<int>>;

Matrix transpose_buildMatrix(int y, int x, bool verbose) {
    if (y <= 0 || x <= 0) {
        printf("Error: Matrix dimensions must be positive integers.\n");
        return {};
    }

    if (verbose) printf("[VERBOSE] Creating a %d by %d matrix.\n", y, x);
    Matrix A(y, vector<int>(x));
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[0].size(); j++) {
            A[i][j] = i * A.size() + j;
        }
    }
    return A;
}

//ensure everything is correctly transposed
bool is_transpose(const Matrix& A, const Matrix& B) {
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[0].size(); ++j) {
            if (A[i][j] != B[j][i]) {
                return false;
            }
        }
    }
    return true;
}

//run any transpose function and print a time
Matrix transpose_time(Matrix (*strategy)(const Matrix& A, bool verbose), const Matrix& A, bool verbose, string name) {
    auto start = chrono::high_resolution_clock::now();
    Matrix B = strategy(A, verbose);
    auto stop = chrono::high_resolution_clock::now();

    if (verbose) printf("[VERBOSE] Verify valid transpose: %s\n", is_transpose(A, B) ? "true" : "false");
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    printf("%s: %lld ms\n", name.c_str(), duration.count());

    return B;
}