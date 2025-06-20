#include "common.h"
using namespace std;

Program::Program(const string &name, cl_context context, const vector<Device*> &devs)
    : program_name(name), context(context), devices(devs) {}

Program::Program(const string &name)
    : program_name(name) {}

void Program::setContext(cl_context ctx) { context = ctx; }
void Program::setDevices(const vector<Device*> &devs) { devices = devs; }

void Program::addKernelFunctions(const KernelFile &file)
{
    for (int i = 0; i < kernel_files.size(); ++i) {
        if (kernel_files[i].getFileName() == file.getFileName())
        {
            cerr << "Kernel file '" << file.getFileName() << "' already added to the program.\n";
            return;
        }
    }

    kernel_files.push_back(file);

    const vector<KernelFunction> &funcs = file.getKernelFunctions();
    for (int i = 0; i < funcs.size(); ++i) {
        const string &name = funcs[i].getFunctionName();
        bool exists = false;

        for (int j = 0; j < kernel_functions.size(); ++j) {
            if (kernel_functions[j].getFunctionName() == name)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
            kernel_functions.push_back(funcs[i]);
        else
            cerr << "Function '" << name << "' is already added to the program!\n";
    }
}

void Program::compileProgram()
{
    vector<string> sources;
    for (int i = 0; i < kernel_files.size(); ++i) {
        sources.push_back(kernel_files[i].getSourceCode());
    }

    vector<const char*> c_sources;
    vector<size_t> source_lengths;

    for (int i = 0; i < sources.size(); ++i) {
        c_sources.push_back(sources[i].c_str());
        source_lengths.push_back(sources[i].length());
    }

    cl_int err;
    program = clCreateProgramWithSource(context, c_sources.size(), c_sources.data(), source_lengths.data(), &err);
    if (err != CL_SUCCESS)
        throw runtime_error("Failed to create cl_program! Error code: " + to_string(err));

    vector<cl_device_id> deviceIds;
    for (int i = 0; i < devices.size(); ++i) {
        deviceIds.push_back(devices[i]->getDeviceId());
    }
    
    err = clBuildProgram(program, deviceIds.size(), deviceIds.data(), nullptr, nullptr, nullptr);        
    if (err != CL_SUCCESS)
    {
        for (int i = 0; i < deviceIds.size(); ++i) {
            size_t logSize;
            clGetProgramBuildInfo(program, deviceIds[i], CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
            vector<char> log(logSize);
            clGetProgramBuildInfo(program, deviceIds[i], CL_PROGRAM_BUILD_LOG, logSize, log.data(), nullptr);
            cerr << "Build log for device " << i << ":\n" << log.data() << '\n';
        }
        throw runtime_error("Failed to build OpenCL program for one or more devices!");
    }

    cout << "Program '" << program_name << "' compiled successfully!\n";
}

cl_program Program::getProgram() const { return program; }
cl_context Program::getContext() const { return context; }
const vector<KernelFunction>& Program::getKernelFunctions() const { return kernel_functions; }
const vector<Device*>& Program::getDevices() const { return devices; }
const string& Program::getProgramName() const { return program_name; }

void Program::showProgramInfo() const
{
    cout << "Program: " << program_name << '\n';
    cout << "Kernel functions:\n";
    for (int i = 0; i < kernel_functions.size(); ++i) {
        kernel_functions[i].showKernelFunctionInfo();
    }
}

Program::~Program()
{
    if (program != nullptr)
    {
        clReleaseProgram(program);
        program = nullptr;
    }
}
