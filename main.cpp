#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

using Matrix = vector<vector<int>>;

Matrix transpose(const Matrix& A) {
    Matrix B(A[0].size(), vector<int>(A.size()));
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[0].size(); j++) {
            B[j][i] = A[i][j];
        }
    }
    return B;
}

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

int main() {
    Matrix A(500, vector<int>(500, 1));
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[0].size(); j++) {
            A[i][j] = i * A.size() + j;
        }
    }

    auto start = high_resolution_clock::now();
    Matrix B = transpose(A);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    printf("Sequential: %d ms\n", duration.count());
    return 0;
}
