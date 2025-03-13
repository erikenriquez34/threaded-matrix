#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <thread>

using namespace std;
using namespace std::chrono;

using Matrix = vector<vector<int>>;
bool verbose = false;

Matrix transpose_sequential(const Matrix& A) {
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
Matrix transpose_multithreaded(const Matrix& A, int threads) {
    Matrix B(A[0].size(), vector<int>(A.size())); //make the x by y matrix

    vector<thread> pool;
    int column_chunk = A[0].size() / threads;
    
    for (int t = 0; t < threads; t++) {
        int start = t * column_chunk;
        int end = (t == threads - 1) ? A[0].size() : (t + 1) * column_chunk;

        printf("[VERBOSE] Thread %d assigned along rows [%d, %d].\n", (t+1), start, end);
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

void printHelp() {
    printf("Usage: ./program_name [-m rows] [-n cols] [-h]\n");
    printf("Options:\n");
    printf("  -m <rows>   Specify the number of rows in the matrix.\n");
    printf("  -n <cols>   Specify the number of columns in the matrix.\n");
    printf("  -v          Enable verbose mode.\n");
    printf("  -h          Display this help message.\n");
}

int main(int argc, char* argv[]) {
    int opt, y, x;
    while ((opt = getopt(argc, argv, "hy:x:v")) != -1) {
        switch(opt) {
            case 'h':
                printHelp();
                return 0;
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
                printf("Use -h or --help for usage information.\n");
                return 1;
        }
    }

    if (y <= 0 || x <= 0) {
        printf("Error: Matrix dimensions must be positive integers.\n");
        return 1;
    }

    if (verbose) printf("[VERBOSE] Creating a %d by %d matrix.\n", y, x);
    Matrix A(y, vector<int>(x));
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[0].size(); j++) {
            A[i][j] = i * A.size() + j;
        }
    }
    if (verbose) printf("[VERBOSE] Done.\n");

    //will do this multiple times for all methods
    auto start = high_resolution_clock::now();
    Matrix B = transpose_sequential(A);
    auto stop = high_resolution_clock::now();

    if (verbose) printf("[VERBOSE] Verify valid transpose: %s\n", is_transpose(A, B) ? "true" : "false");

    auto duration = duration_cast<milliseconds>(stop - start);
    printf("Sequential: %d ms\n", duration.count());

    int threads = thread::hardware_concurrency();
    if (verbose) printf("[VERBOSE] Hardware supplied with %d threads.\n", threads);

    start = high_resolution_clock::now();
    B = transpose_multithreaded(A, threads);
    stop = high_resolution_clock::now();

    if (verbose) printf("[VERBOSE] Verify valid transpose: %s\n", is_transpose(A, B) ? "true" : "false");

    duration = duration_cast<milliseconds>(stop - start);
    printf("Multithreaded: %d ms\n", duration.count());
    return 0;
}
