#include <iostream>
#include <cuda_runtime.h>
#include <opencv2/opencv.hpp>

#define TILE_WIDTH 16

// 1. Tiled Kernel (The Optimized Hardware Path)
__global__ void MatrixMulTiled(float* A, float* B, float* C, int width) {
    __shared__ float ds_A[TILE_WIDTH][TILE_WIDTH];
    __shared__ float ds_B[TILE_WIDTH][TILE_WIDTH];

    int bx = blockIdx.x;  int by = blockIdx.y;
    int tx = threadIdx.x; int ty = threadIdx.y;

    int row = by * TILE_WIDTH + ty;
    int col = bx * TILE_WIDTH + tx;
    float pValue = 0;

    for (int m = 0; m < (width / TILE_WIDTH); ++m) {
        ds_A[ty][tx] = A[row * width + (m * TILE_WIDTH + tx)];
        ds_B[ty][tx] = B[(m * TILE_WIDTH + ty) * width + col];

        __syncthreads(); // Barrier: Ensure tile is loaded

        for (int k = 0; k < TILE_WIDTH; ++k) {
            pValue += ds_A[ty][k] * ds_B[k][tx];
        }

        __syncthreads(); // Barrier: Ensure computation is done
    }
    if (row < width && col < width)
        C[row * width + col] = pValue;
}

int main() {
    int width = 512; 
    size_t size = width * width * sizeof(float);

    // Host allocation
    float *h_A = (float*)malloc(size);
    float *h_B = (float*)malloc(size);
    float *h_C = (float*)malloc(size);

    // Initialize with a gradient for a cool visual effect
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            h_A[i * width + j] = (float)i / width; 
            h_B[i * width + j] = (float)j / width;
        }
    }

    // Device allocation
    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);
    dim3 dimGrid(width / TILE_WIDTH, width / TILE_WIDTH);

    // Timing
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    std::cout << "🚀 Launching Tiled Kernel..." << std::endl;
    cudaEventRecord(start);
    MatrixMulTiled<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, width);
    cudaEventRecord(stop);

    // CRITICAL: Sync and Copy
    cudaDeviceSynchronize(); 
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    float ms = 0;
    cudaEventElapsedTime(&ms, start, stop);
    std::cout << "⏱️ Execution Time: " << ms << " ms" << std::endl;

    // --- OPENCV VISUALIZATION ---
    cv::Mat resultMat(width, width, CV_32FC1, h_C);
    
    double min, max;
    cv::minMaxLoc(resultMat, &min, &max);
    std::cout << "📊 Value Range: [" << min << ", " << max << "]" << std::endl;

    if (max > 0) {
        cv::Mat displayMat, colorHeatmap;
        cv::normalize(resultMat, displayMat, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        cv::applyColorMap(displayMat, colorHeatmap, cv::COLORMAP_JET);
        cv::imwrite("output_heatmap.png", colorHeatmap);
        std::cout << "✅ Heatmap saved as 'output_heatmap.png'" << std::endl;
    } else {
        std::cout << "❌ Error: Matrix is empty (All Blue). Check Kernel/Sync." << std::endl;
    }

    // Cleanup
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    free(h_A); free(h_B); free(h_C);
    return 0;
}
