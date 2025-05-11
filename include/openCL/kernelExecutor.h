#pragma once

#include <CL/cl.h>
#include <string>
#include <vector>
#include "platform/platformDevices.h"

using namespace std;

// Base class for executing OpenCL kernels on a device
class KernelExecutor {
    private:
        // Loads the OpenCL kernel source code from a file based on a file name
        string readKernel(const char* fileName);

        // Creates and builds a program from the kernel source
        // Uses readKernel
        cl_program createProgram(cl_context context, const char* fileName);

    protected:
        PlatformDevices &platformDevice;
        cl_context context;
        cl_command_queue queue;

        // 
        cl_program program;
        
        cl_kernel kernel;
        vector<cl_mem> buffers;
        vector<void*> hostPtrs;
        vector<size_t> sizes;

        virtual string kernelFilename() = 0;
        virtual string kernelFunction() = 0;
        virtual bool prepareData() = 0;
        virtual bool setExtraArgs(cl_uint &argIndex) = 0;

        bool createBuffers();
        bool setKernelArgs();
        bool enqueueKernel();
        bool readResults();
        void cleanupBuffers();

        virtual size_t inputCount() = 0;
        virtual size_t elementSize() = 0;
        virtual size_t globalSize() = 0;

    public:
        KernelExecutor(PlatformDevices &pd, size_t deviceIndex);
        virtual ~KernelExecutor();

        bool initialize();
        bool execute();
};