#include "common.h"

using namespace std;

KernelExecutor::KernelExecutor(Program &program)
    : program(program) {}

Program& KernelExecutor::getProgram() { return program; }

void KernelExecutor::addKernelCall(const string &functionName, const vector<Data*> &args)
{
    execution_list.push_back({functionName, args});
}

void KernelExecutor::allocateBuffers()
{
    cl_context context = program.getContext();

    for (int i = 0; i < execution_list.size(); ++i)
        for (int j = 0; j < execution_list[i].arguments.size(); ++j)
            execution_list[i].arguments[j]->create(context);
    
    cout << '\n';
}

void KernelExecutor::execute_one_device()
{
    cl_context context = program.getContext();
    cl_program clProg = program.getProgram();
    const vector<KernelFunction> &functions = program.getKernelFunctions();
    Device* device = program.getDevices()[0];

    for (int i = 0; i < execution_list.size(); ++i)
    {
        const string &funcName = execution_list[i].functionName;
        const vector<Data*> &args = execution_list[i].arguments;

        const KernelFunction* matchedFunc = nullptr;
        for (int j = 0; j < functions.size(); ++j)
        {
            if (functions[j].getFunctionName() == funcName)
            {
                matchedFunc = &functions[j];
                break;
            }
        }

        if (!matchedFunc)
            throw runtime_error("Function " + funcName + " not found in program!");

        cl_int err;
        cl_kernel kernel = clCreateKernel(clProg, funcName.c_str(), &err);
        if (err != CL_SUCCESS)
            throw runtime_error("Failed to create kernel: " + funcName + ". Error code: " + to_string(err));

        for (int k = 0; k < args.size(); ++k)
        {
            try
            {
                args[k]->setKernelArguments(kernel, k);
            }
            catch(const exception& e)
            {
                throw runtime_error("Kernel argument error at index " + to_string(k) + ": " + e.what());
                clReleaseKernel(kernel);
            }
        }

        cout << "Kernel: " << funcName << '\n';
        
        const vector<size_t> &globalSize = matchedFunc->getGlobalSize();
        const vector<size_t> &localSize = matchedFunc->getLocalSize();
        unsigned int workDim = matchedFunc->getWorkDim();

        const size_t* globalPtr = globalSize.data();
        const size_t* localPtr = (localSize.size() == globalSize.size()) ? localSize.data() : nullptr;

        device->addCommandToQueue(kernel, workDim, const_cast<size_t*>(globalPtr), const_cast<size_t*>(localPtr));

        clReleaseKernel(kernel);
    }
}