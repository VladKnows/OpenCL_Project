#pragma once

#include <CL/cl.h>
#include <string>
#include <vector>
#include "platform/platformDevices.h"
#include "openCL/kernelExecutor.h"

class VectorAdd : public KernelExecutor {
    private:
        size_t dataSize;

    public:
        VectorAdd(PlatformDevices &pd, size_t deviceIndex, size_t dataSize);

        string kernelFilename() override;
        string kernelFunction() override;
        void prepareData() override;
        void setExtraArgs(cl_uint &argIndex)
};