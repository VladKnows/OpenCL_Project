#include "common.h"

Scalar::Scalar(void* valuePtr, size_t size) : data(valuePtr), size(size) {}

void Scalar::setKernelArguments(cl_kernel kernel, cl_uint index) const
{
    cl_int err = clSetKernelArg(kernel, index, size, data);
    if (err != CL_SUCCESS)
        throw runtime_error("Failed to set Scalar as kernel argument! Error code: " + to_string(err));
}

void Scalar::create(cl_context context) { }