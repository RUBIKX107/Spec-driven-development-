#include <iostream>
#include <cuda_runtime.h>

#define TILE_WIDTH 16

__global__ void MatrixMulTiled(float* A, float* B, float* C, int width) {
    // Shared memory for tiles
    __shared__ float ds_A[TILE_WIDTH][TILE_WIDTH];
    __shared__ float ds_B[TILE_WIDTH][TILE_WIDTH];

}
    int bx = blockIdx.x;  int by = blockIdx.y;
    int tx = threadIdx.x; int ty = threadIdx.y;

    // Identify row and column of the C element to work on
    int row = by * TILE_WIDTH + ty;
    int col = bx * TILE_WIDTH + tx;
    float pValue = 0;

    // Loop over the tiles required to compute the C element
    for (int m = 0; m < width/TILE_WIDTH; ++m) {
        // Collaborative loading of tiles into shared memory
        ds_A[ty][tx] = A[row * width + (m * TILE_WIDTH + tx)];
        ds_B[ty][tx] = B[(m * TILE_WIDTH + ty) * width + col];

        // Wait for all threads in the block to finish loading
        __syncthreads();

        for (int k = 0; k < TILE_WIDTH; ++k) {
            pValue += ds_A[ty][k] * ds_B[k][tx];
        }

        // Wait for all threads to finish calculation before loading next tile
        __syncthreads();
    }
    C[row * width + col] = pValue;
}

int main() {
    int width = 512; // Must be multiple of TILE_WIDTH
    size_t size = width * width * sizeof(float);

    float *h_A, *h_B, *h_C;
    h_A = (float*)malloc(size); h_B = (float*)malloc(size); h_C = (float*)malloc(size);

    // Initialize matrices
    for(int i=0; i<width*width; i++) { h_A[i] = 1.0f; h_B[i] = 2.0f; }

    float *d_A, *d_B, *d_C;
    cudaMalloc(&d_A, size); cudaMalloc(&d_B, size); cudaMalloc(&d_C, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    dim3 dimGrid(width/TILE_WIDTH, width/TILE_WIDTH);
    dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);

    MatrixMulTiled<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, width);

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    std::cout << "Done! Element [0,0] is: " << h_C[0] << " (Expected: " << width * 2 << ")" << std::endl;

    return 0;
}