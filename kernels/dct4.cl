__kernel void dct4_f32(
    __global const float* input,
    __global float* output,
    const uint N)
{
    const int n = get_local_id(0);
    const int k = get_global_id(1);
    const int local_size = get_local_size(0);

    __local float partial_sums[256];

    float factor = M_PI_F / (float)N;
    float angle = factor * (n + 0.5f) * (k + 0.5f);
    float contrib = input[n] * cos(angle);
    partial_sums[n] = contrib;

    barrier(CLK_LOCAL_MEM_FENCE);

    for (int stride = local_size / 2; stride > 0; stride /= 2)
    {
        if (n < stride)
            partial_sums[n] += partial_sums[n + stride];
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (n == 0)
        output[k] = partial_sums[0] * sqrt(2.0f / (float)N);
}
