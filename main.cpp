#include <iostream>
#include <vector>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include "transpose.h"

using namespace std;

void printHelp() {
    printf("Usage: ./main [-m rows] [-n cols] [-h]\n");
    printf("Options:\n");
    printf("  -m <rows>   Specify the number of rows in the matrix.\n");
    printf("  -n <cols>   Specify the number of columns in the matrix.\n");
    printf("  -v          Enable verbose mode.\n");
    printf("  -h          Display this help message.\n");
}

void run(const string& exe, int y, int x, bool verbose) {
    stringstream cmd;

    if (verbose) cmd << "perf stat -e cache-misses,cache-references ";
    cmd << "./" << exe << " -y " << y << " -x " << x;
    if (verbose) cmd << " -v";

    if (verbose) printf("Command: %s\n", cmd.str());

    int result = system(cmd.str().c_str());
    if (result != 0) {
        cerr << "Error: Failed to run " << exe << " via perf" << endl;
    }
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
        cerr << "Error: Matrix dimensions must be positive. Use -h for help.\n";
        return 1;
    }
    
    vector<string> executables = {
        "singlethread-oblivious",
        "singlethread-sequential",
        "multithreaded-assign"
    };

    for (const auto& exe : executables) {
        run(exe, y, x, verbose);
    }

    return 0;
}
