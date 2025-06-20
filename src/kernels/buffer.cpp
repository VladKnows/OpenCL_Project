#include "common.h"


using namespace std;

Buffer::Buffer(size_t count, size_t elementSize, Access access, void* hostPtr)
    : size(count * elementSize), access(access), hostData(hostPtr) {}

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

    if (hostData)
        flags |= CL_MEM_COPY_HOST_PTR;

    mem = clCreateBuffer(context, flags, size, hostData, &err);
    if (err != CL_SUCCESS)
        throw runtime_error("Failed to create OpenCL buffer! Error code: " + to_string(err));        
}

cl_mem Buffer::getCLBuffer(const cl_context& context) const
{
    if (!mem)
        const_cast<Buffer*>(this)->create(context);
    return mem;
}

Buffer::~Buffer()
{
    if (mem)
        clReleaseMemObject(mem);
}