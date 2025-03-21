#include <iostream>
#include <unistd.h>
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
    if (!A.empty()) {Matrix B = transpose_time(transpose_sequential, A, verbose, "Singlethread Sequential");}

    return 0;
}