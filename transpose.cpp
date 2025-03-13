#include <iostream>
#include <vector>
#include <thread>

using namespace std;
using Matrix = vector<vector<int>>;

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

//tranpose sections as directed by multithreaded runner
void transpose_worker(const Matrix& A, Matrix& B, int start_row, int end_row) {
    int y = B[0].size(); //new column size (y)
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < y; j++) {
            B[i][j] = A[j][i];
        }
    }
}

//assign threads to sections of column A, which are the rows of B
Matrix transpose_multithreaded(const Matrix& A, int threads, bool verbose) {
    Matrix B(A[0].size(), vector<int>(A.size())); //make the x by y matrix

    vector<thread> pool;
    int column_chunk = A[0].size() / threads;
    
    for (int t = 0; t < threads; t++) {
        int start = t * column_chunk;
        int end = (t == threads - 1) ? A[0].size() : (t + 1) * column_chunk;

        if (verbose) printf("[VERBOSE] Thread %d assigned along rows [%d, %d].\n", (t+1), start, end);
        pool.emplace_back(transpose_worker, cref(A), ref(B), start, end);
    }

    for (auto& th : pool) {
        th.join();
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