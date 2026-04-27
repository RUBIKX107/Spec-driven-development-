#include <iostream>
#include <cuda_runtime.h>

#define TILE_WIDTH 16

// 1. NAIVE KERNEL
__global__ void MatrixMulNaive(float* A, float* B, float* C, int width) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < width && col < width) {
        float pValue = 0;
        for (int k = 0; k < width; ++k) {
            pValue += A[row * width + k] * B[k * width + col];
        }
        C[row * width + col] = pValue;
    }
}

// 2. TILED KERNEL (Uses Shared Memory)
__global__ void MatrixMulTiled(float* A, float* B, float* C, int width) {
    __shared__ float ds_A[TILE_WIDTH][TILE_WIDTH];
    __shared__ float ds_B[TILE_WIDTH][TILE_WIDTH];

    int bx = blockIdx.x;  int by = blockIdx.y;
    int tx = threadIdx.x; int ty = threadIdx.y;

    int row = by * TILE_WIDTH + ty;
    int col = bx * TILE_WIDTH + tx;
    float pValue = 0;

    for (int m = 0; m < (width / TILE_WIDTH); ++m) {
        // Load tiles into shared memory
        ds_A[ty][tx] = A[row * width + (m * TILE_WIDTH + tx)];
        ds_B[ty][tx] = B[(m * TILE_WIDTH + ty) * width + col];

        __syncthreads(); // Wait for all threads to finish loading

        for (int k = 0; k < TILE_WIDTH; ++k) {
            pValue += ds_A[ty][k] * ds_B[k][tx];
        }

        __syncthreads(); // Wait for all threads to finish computing before loading next tile
    }
    C[row * width + col] = pValue;
}

int main() {
    int width = 512; // Must be a multiple of TILE_WIDTH
    size_t size = width * width * sizeof(float);

    // Allocate Host Memory
    float *h_A = (float*)malloc(size);
    float *h_B = (float*)malloc(size);
    float *h_C = (float*)malloc(size);

    // Initialize data
    for (int i = 0; i < width * width; i++) {
        h_A[i] = 1.0f;
        h_B[i] = 2.0f;
    }

    // Allocate Device Memory
    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    // Copy to Device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Launch Config
    dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);
    dim3 dimGrid(width / TILE_WIDTH, width / TILE_WIDTH);

    // Timing Setup
    cudaEvent_t start, stop;
    float ms_naive = 0, ms_tiled = 0;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // --- RUN NAIVE ---
    cudaEventRecord(start);
    MatrixMulNaive<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, width);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms_naive, start, stop);

    // --- RUN TILED ---
    cudaEventRecord(start);
    MatrixMulTiled<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, width);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms_tiled, start, stop);

    // Display Results
    std::cout << "Matrix Width: " << width << "x" << width << std::endl;
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Naive MatMul Time: " << ms_naive << " ms" << std::endl;
    std::cout << "Tiled MatMul Time: " << ms_tiled << " ms" << std::endl;
    std::cout << "Speedup: " << ms_naive / ms_tiled << "x" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    // Verify one element
    std::cout << "Validation - Top Left Element: " << h_C[0] << " (Expected " << width * 2 << ")" << std::endl;

    // Cleanup
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    free(h_A); free(h_B); free(h_C);
    cudaEventDestroy(start); cudaEventDestroy(stop);

    return 0;
}