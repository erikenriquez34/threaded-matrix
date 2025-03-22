#include <iostream>
#include <unistd.h>
#include "transpose.h"

using namespace std;

void transpose_worker(const Matrix& A, Matrix& B, int y, int x, int offsety, int offsetx) {
    if (y <= 32) { //small enough transpose it
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                B[offsetx + j][offsety + i] = A[offsety + i][offsetx + j];
            }
        }
    } else { //build new submatrix
        int newy = y / 2;
        int newx = x / 2;

        transpose_worker(A, B, newy, newx, offsety, offsetx);
        transpose_worker(A, B, newy, newx, offsety, offsetx + newx);
        transpose_worker(A, B, newy, newx, offsety + newy, offsetx);
        transpose_worker(A, B, newy, newx, offsety + newy, offsetx + newx);

        //transpose remaining odd-sized parts (if needed)
        if (y % 2 != 0) {
            for (int i = 0; i < x; i++) {
                B[offsetx + i][offsety + y - 1] = A[offsety + y - 1][offsetx + i];
            }
        }

        if (x % 2 != 0) {
            for (int i = 0; i < y; i++) {
                B[offsetx + x - 1][offsety + i] = A[offsety + i][offsetx + x - 1];
            }
        }
    }
}

Matrix transpose_oblivious(const Matrix& A, bool verbose) {
    if (verbose) printf("[VERBOSE] Starting oblivious matrix transpose.\n");
    Matrix B(A[0].size(), vector<int>(A.size())); //make the x by y matrix
    transpose_worker(A, B, A.size(), A[0].size(), 0, 0);
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
    if (!A.empty()) {Matrix B = transpose_time(transpose_oblivious, A, verbose, "Singlethread Cache Oblivious");}

    return 0;
}