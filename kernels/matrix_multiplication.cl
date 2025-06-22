__kernel void matrix_multiplication_f32(
    __global float* A,
    __global float* B,
    __global float* C,
    const uint M,
    const uint N,
    const uint P)
{
    int row = get_global_id(0);
    int col = get_global_id(1);

    if (row < M && col < P) {
        float sum = 0.0f;
        for (uint k = 0; k < N; ++k) {
            sum += A[row * N + k] * B[k * P + col];
        }
        C[row * P + col] = sum;
    }
}