#include "common.h"

using namespace std;

KernelFunction::KernelFunction(const string &name, unsigned int workDim, const vector<size_t> &globalSizes, const vector<size_t> &localSizes)
    : function_name(name), global_size(globalSizes), local_size(localSizes), workDim(workDim)
{
    if (global_size.size() != workDim)
        throw runtime_error("Global size vector size must match workDim!\n");
}

KernelFunction::KernelFunction(const string &name, size_t globalSize, size_t localSize)
    : function_name(name), global_size({globalSize}), local_size({localSize}), workDim(1) { }

const string &KernelFunction::getFunctionName() const { return function_name; }
const vector<size_t>& KernelFunction::getGlobalSize() const { return global_size; }
const vector<size_t>& KernelFunction::getLocalSize() const { return local_size; }
const unsigned int &KernelFunction::getWorkDim() const { return workDim; }

void KernelFunction::showKernelFunctionInfo() const
{
    cout << "\tKernel function: " << function_name << ", Global size: [";
    for (int i = 0; i < global_size.size(); ++i)
    {
        cout << global_size[i];
        if (i < global_size.size() - 1)
            cout << ", ";
    }
    cout << "], Local size: [";
    if (local_size.empty())
    {
        cout << "auto";
    }
    else
    {
        for (int i = 0; i < local_size.size(); ++i)
        {
            cout << local_size[i];
            if (i < local_size.size() - 1)
                cout << ", ";
        }
    }
    cout << "]\n";
}