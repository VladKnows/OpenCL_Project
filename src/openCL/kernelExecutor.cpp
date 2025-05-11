#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <CL/cl.h>
#include <string>
#include <vector>
#include "platform/platformDevices.h"
#include "openCL/kernelExecutor.h"

string KernelExecutor::readKernel(const char* fileName)
{
    ifstream kernelFile(fileName);
    stringstream kernelSource;

    if (kernelFile)
        kernelSource << kernelFile.rdbuf();
    else
    {
        cerr << "Failed to open kernel file!" << '\n';
        return "";
    }

    return kernelSource.str();
}

cl_program KernelExecutor::createProgram(cl_context context, const char* fileName)
{
    string source = readKernel(fileName);
    const char* source_cstr = source.c_str();
    
    cl_int err;
    cl_program program = clCreateProgramWithSource(context, 1, &source_cstr, nullptr, &err);
    if (err != CL_SUCCESS) {
        cerr << "Failed to create program from source. Error: " << err << '\n';
        return nullptr;
    }

    err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        cerr << "Failed to build program. Error: " << err << '\n';
        
        size_t size;
        clGetProgramBuildInfo(program, 0, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size);
        char* log = new char[size];
        clGetProgramBuildInfo(program, 0, CL_PROGRAM_BUILD_LOG, size, log, nullptr);
        cerr << log << '\n';
        delete[] log;

        return nullptr;
    }

    return program;
}

KernelExecutor::KernelExecutor(PlatformDevices &pd, size_t deviceIndex = 0) : platformDevice(pd)
{
    context = pd.getContext();
    queue = pd.getQueues().at(deviceIndex);
    program = nullptr;
    kernel = nullptr;
}

KernelExecutor::~KernelExecutor()
{
    if(kernel) clReleaseKernel(kernel);
    if(program) clReleaseProgram(program);
}

bool KernelExecutor::initialize()
{
    program = createProgram(context, kernelFilename().c_str());
    if (!program)
    {
        cerr << "Program couldn't be created!\n";
        return false;
    }

    cl_int err;
    kernel = clCreateKernel(program, kernelFunction().c_str(), &err);
    if(err != CL_SUCCESS)
    {
        cerr << "Kernel couldn't be created!\n";
        return false;
    }

    return true;
}

bool KernelExecutor::execute()
{
    if (!prepareData())
    {
        cerr << "Couldn't prepare data!\n";
        return false;
    }

    if (!createBuffers())
    {
        cerr << "Couldn't create buffers!\n";
        return false;
    }

    if (!setKernelArgs())
    {
        cerr << "Couldn't set Kernel arguments!\n";
        return false;
    }

    if (!enqueueKernel())
    {
        cerr << "Couldn't load kernel!\n";
        return false;
    }

    if (!readResults())
    {
        cerr << "Couldn't read results!\n";
        return false;
    }

    cleanupBuffers();
    
    return true;
}

bool KernelExecutor::createBuffers()
{
    buffers.resize(hostPtrs.size());
    cl_int err;
    for (size_t i = 0; i < hostPtrs.size(); ++i) {
        cl_mem_flags flags;
        if(i < inputCount())
            flags = CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR;
        else
            flags = CL_MEM_WRITE_ONLY;
        
        buffers[i] = clCreateBuffer(context, flags, sizes[i], hostPtrs[i], &err);
        
        if (err != CL_SUCCESS) 
            return false;
    }
    return true;
}

bool KernelExecutor::setKernelArgs()
{
    cl_uint arg = 0;
    cl_int err;

    for (auto &buf : buffers) {
        err = clSetKernelArg(kernel, arg++, sizeof(cl_mem), &buf);

        if (err != CL_SUCCESS)
            return false;
    }

    return setExtraArgs(arg);
}

bool KernelExecutor::enqueueKernel() {
    size_t global = globalSize();
    cl_int err = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &global, nullptr, 0, nullptr, nullptr);
    
    if (err != CL_SUCCESS)
        return false;
    
    clFinish(queue);
    return true;
}

bool KernelExecutor::readResults() {
    cl_int err;
    for (size_t i = inputCount(); i < buffers.size(); ++i) {
        err = clEnqueueReadBuffer(queue, buffers[i], CL_TRUE, 0, sizes[i], hostPtrs[i], 0, nullptr, nullptr);
        
        if (err != CL_SUCCESS)
            return false;
    }

    return true;
}

void KernelExecutor::cleanupBuffers() {
    for (auto &b : buffers)
        clReleaseMemObject(b);
}