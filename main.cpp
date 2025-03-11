#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>

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

void printHelp() {
    printf("Usage: ./program_name [-m rows] [-n cols] [-h]\n");
    printf("Options:\n");
    printf("  -m <rows>   Specify the number of rows in the matrix.\n");
    printf("  -n <cols>   Specify the number of columns in the matrix.\n");
    printf("  -h          Display this help message.\n");
}

int main(int argc, char* argv[]) {
    int opt, m, n;
    while ((opt = getopt(argc, argv, "m:n:h")) != -1) {
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
            case ':':
                printf("Missing argument for option: %c\n", (char)optopt);
                return 1;
            default:
                printf("Use -h or --help for usage information.\n");
                return 1;
        }
    }

    Matrix A(m, vector<int>(n));
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
