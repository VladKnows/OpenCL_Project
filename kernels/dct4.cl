__kernel void dct4_f32(
    __global const float* input,
    __global float* output,
    const uint N
)
{
    int k = get_global_id(0);
    if (k >= N) return;

    float result = 0.0f;
    float factor = M_PI_F / (float)N;

    for (uint n = 0; n < N; ++n)
    {
        float angle = factor * (n + 0.5f) * (k + 0.5f);
        result += input[n] * cos(angle);
    }

    output[k] = result * sqrt(2.0f / (float)N);
}
