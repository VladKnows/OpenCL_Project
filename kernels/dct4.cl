__kernel void dct4_f32(
    __global const float* input,
    __global float* output,
    const uint N)
{
    const int lid = get_local_id(0);
    const int k = get_global_id(1);
    const int local_size = get_local_size(0);

    __local float local_sum[256];
    float sum = 0.0f;
    float factor = M_PI_F / (float)N;

    for (int n = lid; n < N; n += local_size)
    {
        float angle = factor * (n + 0.5f) * (k + 0.5f);
        sum += input[n] * cos(angle);
    }

    local_sum[lid] = sum;
    barrier(CLK_LOCAL_MEM_FENCE);

    for (int stride = local_size / 2; stride > 0; stride /= 2)
    {
        if (lid < stride)
            local_sum[lid] += local_sum[lid + stride];
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (lid == 0)
        output[k] = local_sum[0] * sqrt(2.0f / (float)N);
}
