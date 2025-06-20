__kernel void vector_add(__global float* A, __global float* B, __global float* C, __global const unsigned int *N) {
    int i = get_global_id(0);
    if (i < *N)
        C[i] = A[i] + B[i];
}