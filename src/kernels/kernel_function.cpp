#include "common.h"
using namespace std;

KernelFunction::KernelFunction(const string &name, size_t global, size_t local)
    : functionName(name), globalSize(global), localSize(local) {}

const string &KernelFunction::getFunctionName() const { return functionName; }
const size_t &KernelFunction::getGlobalSize() const { return globalSize; }
const size_t &KernelFunction::getLocalSize() const { return localSize; }

void KernelFunction::showKernelFunctionInfo() const
{
    cout << "\tKernel function: " << functionName
        << ", Global size: " << globalSize
        << ", Local size: " << (localSize > 0 ? to_string(localSize) : "auto") << "\n";
}