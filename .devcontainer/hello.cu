#include <iostream>
#include <cuda_runtime.h>

// This is the "Kernel" - it runs on the GPU
__global__ void add(int a, int b, int *c) {
    *c = a + b;
}

int main() {
    int c; // host copy of c
    int *dev_c; // device copy of c
    
    // 1. Allocate memory on the GPU
    cudaMalloc((void**)&dev_c, sizeof(int));

    // 2. Launch the kernel on the GPU (1 block, 1 thread)
    add<<<1, 1>>>(2, 7, dev_c);

    // 3. Copy the result back from GPU to CPU
    cudaMemcpy(&c, dev_c, sizeof(int), cudaMemcpyDeviceToHost);

    std::cout << "2 + 7 = " << c << " (Computed on GPU... hopefully!)" << std::endl;

    // 4. Free the memory
    cudaFree(dev_c);

    return 0;
}