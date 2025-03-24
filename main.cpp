#include <iostream>
#include <vector>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
#include "transpose.h"

using namespace std;

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
    
    return 0;
}
