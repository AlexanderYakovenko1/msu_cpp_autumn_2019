#include <iostream>
#include <random>
#include "timer.h"

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        printf("%s [0, 1] arr_size\n0 - sum by rows, 1 - sum by columns\narr_size - size of the array", argv[0]);
        return 1;
    }

    int mode, arr_size;
    sscanf(argv[1], "%d", &mode);
    sscanf(argv[2], "%d", &arr_size);

    if (mode == 0 || mode == 1) {
        if (arr_size <= 0) {
            printf("Invalide arr_size value: %d\n", arr_size);
            return 1;
        }
        double **arr = static_cast<double**>(malloc(arr_size * sizeof(*arr)));
        for (int i = 0; i < arr_size; ++i) {
            arr[i] = static_cast<double*>(malloc(arr_size * sizeof(**arr)));
        }
        volatile double sum = 0;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution <double> uid(-1., 1.);
        
        for (int i = 0; i < arr_size; ++i) {
            for (int j = 0; j < arr_size; ++j) {
                arr[i][j] = uid(gen);
            }
        }

        if (mode) {
            printf("cols\n");
            Timer t;
            for (int i = 0; i < arr_size; ++i) {
                for (int j = 0; j < arr_size; ++j) {
                    sum += arr[i][j];
                }
            }
        } else {
            printf("rows\n");
            Timer t;
            for (int i = 0; i < arr_size; ++i) {
                for (int j = 0; j < arr_size; ++j) {
                    sum += arr[j][i];
                }
            }
        }
        for (int i = 0; i < arr_size; ++i) {
            free(arr[i]);
        }
        free(arr);
    } else {
        printf("Unknown option: %d\n", mode);
    }
    return 0;
}