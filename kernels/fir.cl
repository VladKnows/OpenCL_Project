__kernel void fir_f32(
    __global const float* coeffs,
    __global float* state,
    __global float* output,
    const uint numTaps,
    const uint blockSize
)
{
    int gid = get_global_id(0);
    if (gid >= blockSize)
        return;

    float acc = 0.0f;
    for (uint i = 0; i < numTaps; ++i) {
        acc += coeffs[i] * state[gid + i];
    }

    output[gid] = acc;
}