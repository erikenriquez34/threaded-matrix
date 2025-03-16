#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <thread>
#include "transpose.h"

using namespace std;
using namespace std::chrono;

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
    bool verbose = false;

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
    Matrix B = transpose_sequential(A, verbose);
    auto stop = high_resolution_clock::now();

    if (verbose) printf("[VERBOSE] Verify valid transpose: %s\n", is_transpose(A, B) ? "true" : "false");

    auto duration = duration_cast<milliseconds>(stop - start);
    printf("Sequential: %d ms\n", duration.count());

    int threads = thread::hardware_concurrency();
    if (verbose) printf("[VERBOSE] Hardware supplied with %d threads.\n", threads);

    start = high_resolution_clock::now();
    B = transpose_multithreaded(A, threads, verbose);
    stop = high_resolution_clock::now();

    if (verbose) printf("[VERBOSE] Verify valid transpose: %s\n", is_transpose(A, B) ? "true" : "false");

    duration = duration_cast<milliseconds>(stop - start);
    printf("Multithreaded: %d ms\n", duration.count());
    
    return 0;
}
