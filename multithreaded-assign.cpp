#include <iostream>
#include <unistd.h>
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
    
    if (verbose) printf("[VERBOSE] ");
    for (int t = 0; t < threads; t++) {
        int start = t * column_chunk;
        int end = (t == threads - 1) ? A[0].size() : (t + 1) * column_chunk;

        if (verbose) printf("Thread %d: [%d, %d), ", (t+1), start, end);
        pool.emplace_back(transpose_worker, cref(A), ref(B), start, end);
    }
    if (verbose) printf("\n");

    for (auto& th : pool) {
        th.join();
    }

    return B;
}

int main(int argc, char* argv[]) {
    int opt, y, x;
    bool verbose = false;

    while ((opt = getopt(argc, argv, "y:x:v")) != -1) {
        switch(opt) {
            case 'y':
                y = atoi(optarg);
                break;
            case 'x':
                x = atoi(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            case ':':
                printf("Missing argument for option: %c\n", (char)optopt);
                return 1;
            default:
                printf("Missing arguements.\n");
                return 1;
        }
    }

    Matrix A = transpose_buildMatrix(y, x, verbose);
    if (!A.empty()) {Matrix B = transpose_time(transpose_multithreaded, A, verbose, "Multithreaded Assign");}

    return 0;
}