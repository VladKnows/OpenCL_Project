__kernel void vector_add_f32(__global float* A,
                         __global float* B,
                         __global float* C,
                         const unsigned int N)
{
    int i = get_global_id(0);
    if (i < N)
        C[i] = A[i] + B[i];
}