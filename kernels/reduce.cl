__kernel void reduce_f32_stage1(
    __global const float* input,
    __global float* partialSums,
    const uint N)
{
    const int lid = get_local_id(0);
    const int gid = get_global_id(0);
    const int group_id = get_group_id(0);
    const int group_size = get_local_size(0);

    __local float local_data[256];

    float value = (gid < N) ? input[gid] : 0.0f;
    local_data[lid] = value;

    barrier(CLK_LOCAL_MEM_FENCE);

    for (int stride = group_size / 2; stride > 0; stride /= 2)
    {
        if (lid < stride)
            local_data[lid] += local_data[lid + stride];
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (lid == 0)
        partialSums[group_id] = local_data[0];
}

__kernel void reduce_f32_stage2(
    __global const float* partialSums,
    __global float* result,
    const uint numGroups)
{
    const int lid = get_local_id(0);

    __local float local_data[256];

    float value = (lid < numGroups) ? partialSums[lid] : 0.0f;
    local_data[lid] = value;

    barrier(CLK_LOCAL_MEM_FENCE);

    for (int stride = get_local_size(0) / 2; stride > 0; stride /= 2)
    {
        if (lid < stride)
            local_data[lid] += local_data[lid + stride];
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (lid == 0)
        result[0] = local_data[0];
}
