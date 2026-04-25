ASSIGNMENT – 6
CUDA Programming Report

Part A (Discriptive part)

Objective

The objective of this task is to explore the hardware characteristics of a GPU using CUDA device query.
This helps in understanding the computational capabilities, memory structure, 
and limitations of the GPU, which are important for designing efficient parallel programs.

Description

A CUDA device query program is used to obtain detailed information about the GPU. 
It retrieves properties such as device name, compute capability, memory sizes, warp size, and execution limits. 
These parameters help in optimizing CUDA programs based on hardware constraints.

Since the system used for development does not have a CUDA-enabled GPU,
the program was executed on a GPU-enabled environment, and standard values are reported.
Array Sum using CUDA

Device Information (NVIDIA Tesla T4 – Google Colab)

GPU Name: NVIDIA Tesla T4
Architecture: Turing
Compute Capability: 7.5
Total Global Memory: 15,999 MB (~16 GB)
Shared Memory per Block: 48 KB
Constant Memory: 64 KB
Warp Size: 32
Max Threads per Block: 1024
Max Block Dimensions: 1024 × 1024 × 64
Max Grid Dimensions: 2147483647 × 65535 × 65535
Multiprocessor Count: 40
Double Precision Support: Yes

Description

The CUDA device query program was executed on Google Colab using an NVIDIA Tesla T4 GPU. 
The Tesla T4 is based on the Turing architecture and supports a compute capability of 7.5, enabling efficient parallel processing.
The GPU provides approximately 16 GB of global memory, allowing large datasets to be processed efficiently. 
Each block has access to 48 KB of shared memory, which is significantly faster than global memory and is used for optimizing performance. 
Additionally, 64 KB of constant memory is available for storing read-only data.

The GPU operates with a warp size of 32, meaning that 32 threads execute simultaneously in a single instruction cycle.
It supports up to 1024 threads per block, with maximum block dimensions of 1024 × 1024 × 64. The grid dimensions allow for large-scale parallel execution.

The Tesla T4 contains 40 streaming multiprocessors, enabling high computational throughput.
It also supports double precision operations, making it suitable for scientific and engineering applications.

Part B: Array Sum using CUDA

Objective


The objective of this task is to compute the sum of elements of a large array using CUDA, demonstrating parallel reduction on a GPU.

Methodology

The array is first initialized on the host system. 
Memory is then allocated on the GPU, and the data is transferred from host to device. 
The computation is performed using multiple threads, where each thread processes a portion of the array. 
Shared memory is utilized within thread blocks to perform partial summations efficiently.
These partial results are then combined to produce the final sum. 
After computation, the result is copied back to the host, and allocated memory is released.

Working Principle

Each thread loads an element of the array into shared memory. 
A parallel reduction technique is applied within each block, where elements are iteratively combined in a tree-like fashion.
Synchronization is used to ensure correct execution. Finally, results from different blocks are accumulated to obtain the overall sum.

Result and Analysis

The array sum is computed efficiently using parallel execution. 
Compared to a serial implementation, the CUDA version significantly reduces execution time for large datasets. 
The use of shared memory and synchronization enhances performance by minimizing global memory access.

Part c Matrix Addition using CUDA

Objective

The objective of this task is to perform addition of two large matrices using CUDA and analyze the computational and memory requirements.

Methodology

Two matrices are initialized on the host system. 
Memory is allocated on the GPU, and both matrices are transferred to the device. 
A two-dimensional grid of threads is used, where each thread computes a single element of the resulting matrix.
After execution, the result is copied back to the host, and all allocated memory is freed.

Working Principle

Each thread is responsible for computing one element of the output matrix.
Using block and grid indexing, threads are mapped to matrix positions. 
This allows simultaneous computation of all matrix elements, significantly improving performance over serial execution.

Performance Analysis

Floating Point Operations:
For a matrix of size N×N, the total number of operations is:

N×N

Global Memory Reads:
Each thread reads one element from each input matrix:

2×N×N

Global Memory Writes:
Each thread writes one element to the output matrix:

N×N

Result and Analysis

Matrix addition is efficiently executed using parallel processing.
The use of a two-dimensional grid structure ensures that each matrix element is processed independently, 
leading to faster execution times compared to CPU-based methods.

Conclusion 

CUDA provides a highly efficient way to perform matrix operations by leveraging parallelism. 
The distribution of computation across multiple threads allows large matrices to be processed quickly, making GPU computing highly advantageous for such tasks.

Final Conclusion

This assignment highlights the advantages of GPU computing using CUDA.
Both array summation and matrix addition demonstrate significant performance improvements when parallelized.
Efficient memory usage, thread organization, and synchronization play a key role in achieving optimal performance.

Performance Comparison

![Performance Graph](https://github.com/user-attachments/assets/2101c79c-11b0-4a85-bca1-634198d2783c)

The above graph compares the execution time of CPU (serial) and GPU (CUDA) implementations. It is observed that:

For smaller input sizes, CPU performance is comparable or slightly better due to GPU overhead.
As the input size increases, GPU performance improves significantly.
GPU achieves better scalability due to parallel execution across multiple threads.

This demonstrates that CUDA is highly effective for large-scale computations.

