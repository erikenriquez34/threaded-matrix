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
    Matrix B(A[0].size(), vector<int>(A.size()));
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[0].size(); j++) {
            B[j][i] = A[i][j];
        }
    }
    if (verbose) printf("[VERBOSE] Tranpose complete.\n");
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
    int opt, m, n;
    while ((opt = getopt(argc, argv, "hm:n:v")) != -1) {
        switch(opt) {
            case 'h':
                printHelp();
                return 0;
            case 'm':
                m = atoi(optarg);
                break;
            case 'n':
                n = atoi(optarg);
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

    if (m <= 0 || n <= 0) {
        printf("Error: Matrix dimensions must be positive integers.\n");
        return 1;
    }

    if (verbose) printf("[VERBOSE] Creating a %d by %d matrix.\n", m, n);
    Matrix A(m, vector<int>(n));
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
    return 0;
}
