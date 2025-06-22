#include "common.h"

using namespace std;

Buffer::Buffer(size_t count, size_t elementSize, Access access, void* hostPtr)
    : size(count * elementSize), access(access), host_data(hostPtr)
{
    type = DataType::Buffer;
}

void Buffer::create(cl_context context)
{
    cl_int err;
    cl_mem_flags flags = 0;

    switch (access) {
        case Access::ReadOnly:
            flags = CL_MEM_READ_ONLY;
            break;
        case Access::WriteOnly:
            flags = CL_MEM_WRITE_ONLY;
            break;
        case Access::ReadWrite:
            flags = CL_MEM_READ_WRITE;
            break;
    }

    if (host_data)
        flags |= CL_MEM_COPY_HOST_PTR;

    auto start = chrono::high_resolution_clock::now();
    mem = clCreateBuffer(context, flags, size, host_data, &err);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << "Copy data in Buffer duration: " << duration.count() << " ms\n";

    if (err != CL_SUCCESS)
        throw runtime_error("Failed to create OpenCL buffer! Error code: " + to_string(err));        
}

void Buffer::setKernelArguments(cl_kernel kernel, cl_uint index) const
{
    cl_int err = clSetKernelArg(kernel, index, sizeof(cl_mem), &mem);
    if (err != CL_SUCCESS)
        throw runtime_error("Failed to set Buffer as kernel argument! Error code: " + to_string(err));
}

Buffer::~Buffer()
{
    if (mem)
        clReleaseMemObject(mem);
}