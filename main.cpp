#include <iostream>
#include <cstdlib>
#include "correlate.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <rows> <cols>\n";
        return 1;
    }

    int ny = atoi(argv[1]);
    int nx = atoi(argv[2]);

    float* data = new float[ny * nx];
    float* result = new float[ny * ny];

    // Initialize data
    for (int i = 0; i < ny * nx; i++)
        data[i] = static_cast<float>(rand()) / RAND_MAX;

    correlate(ny, nx, data, result);

    std::cout << "Correlation computed successfully\n";

    delete[] data;
    delete[] result;
    return 0;
}

