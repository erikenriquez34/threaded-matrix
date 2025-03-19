#include <iostream>
#include <thread>
#include "transpose.h"

using namespace std;

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
Matrix transpose_multithreaded(const Matrix& A, bool verbose) {
    Matrix B(A[0].size(), vector<int>(A.size())); //make the x by y matrix
    int threads = thread::hardware_concurrency();
    if (verbose) printf("[VERBOSE] Transposing with %d threads.\n", threads);

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

int main() {
    Matrix A = transpose_buildMatrix(1000, 1000, true);
    Matrix B = transpose_time(transpose_multithreaded, A, true, "Multithreaded Assign");
    return 0;
}