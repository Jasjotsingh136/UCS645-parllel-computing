#include <cmath>
#include "correlate.h"
#include <omp.h>
void correlate(int ny, int nx, const float* data, float* result) {
   #pragma omp parallel for 
    for (int i = 0; i < ny; i++) {
        for (int j = 0; j <= i; j++) {

            double mean_i = 0.0, mean_j = 0.0;

            for (int k = 0; k < nx; k++) {
                mean_i += data[k + i * nx];
                mean_j += data[k + j * nx];
            }

            mean_i /= nx;
            mean_j /= nx;

            double num = 0.0, den_i = 0.0, den_j = 0.0;

            for (int k = 0; k < nx; k++) {
                double xi = data[k + i * nx] - mean_i;
                double xj = data[k + j * nx] - mean_j;
                num += xi * xj;
                den_i += xi * xi;
                den_j += xj * xj;
            }

            result[i + j * ny] = num / sqrt(den_i * den_j);
        }
    }
}

