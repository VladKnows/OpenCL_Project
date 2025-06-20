#include "common.h"
using namespace std;

KernelExecutor::KernelExecutor(Program &program)
    : program(program) {}

Program& KernelExecutor::getProgram() { return program; }

void KernelExecutor::addKernelCall(const string &functionName, const vector<Buffer*> &args)
{
    executionList.push_back({functionName, args});
}

void KernelExecutor::allocateBuffers()
{
    cl_context context = program.getContext();

    for (int i = 0; i < executionList.size(); ++i) {
        for (int j = 0; j < executionList[i].arguments.size(); ++j) {
            executionList[i].arguments[j]->create(context);
        }
    }
}

void KernelExecutor::execute()
{
    cl_context context = program.getContext();
    cl_program clProg = program.getProgram();
    const vector<KernelFunction> &functions = program.getKernelFunctions();
    Device* device = program.getDevices()[0];

    for (int i = 0; i < executionList.size(); ++i) {
        const string &funcName = executionList[i].functionName;
        const vector<Buffer*> &args = executionList[i].arguments;

        size_t globalSize = 0, localSize = 0;
        bool found = false;
        for (int j = 0; j < functions.size(); ++j) {
            if (functions[j].getFunctionName() == funcName)
            {
                globalSize = functions[j].getGlobalSize();
                localSize = functions[j].getLocalSize();
                found = true;
                break;
            }
        }

        if (!found)
        {
            cerr << "Function " << funcName << " not found in program.\n";
            continue;
        }

        cl_int err;
        cl_kernel kernel = clCreateKernel(clProg, funcName.c_str(), &err);
        if (err != CL_SUCCESS)
        {
            cerr << "Failed to create kernel: " << funcName << ". Error code: " << err << '\n';
            continue;
        }

        for (int k = 0; k < args.size(); ++k) {
            cl_mem mem = args[k]->getCLBuffer(context);
            
            err = clSetKernelArg(kernel, k, sizeof(cl_mem), &mem);
            if (err != CL_SUCCESS)
            {
                cerr << "Failed to set argument " << k << " for kernel " << funcName << ".\n";
                clReleaseKernel(kernel);
                continue;
            }
        }

        const size_t *localPtr = (localSize > 0 ? &localSize : nullptr);
        device->addCommandToQueue(kernel, 1, &globalSize, const_cast<size_t *>(localPtr));

        clReleaseKernel(kernel);
    }
}