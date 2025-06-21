#include "common.h"

using namespace std;

KernelFile::KernelFile(const string &file) : file_name(file)
{
    if (!kernelFileExists())
        throw runtime_error("Kernel file does not exist: " + file_name);

    readSourceCode();
}

bool KernelFile::kernelFileExists() const
{
    ifstream file("kernels/" + file_name);
    return file.good();
}

void KernelFile::readSourceCode()
{
    ifstream file("kernels/" + file_name);
    if (!file.is_open())
        throw runtime_error("Failed to open kernel source file: " + file_name);

    stringstream buffer;
    buffer << file.rdbuf();
    source_code = buffer.str();
}

bool KernelFile::kernelFunctionExists(const string &function) const
{
    string search = "__kernel void " + function;
    return source_code.find(search) != string::npos;
}

void KernelFile::addKernelFunction(const string &functionName, size_t globalSize, size_t localSize)
{
    if (!kernelFunctionExists(functionName))
    {
        throw runtime_error("Kernel function '" + functionName + "' not found in file '" + file_name + "'");
    }

    for (int i = 0; i < functions.size(); ++i)
    {
        if (functions[i].getFunctionName() == functionName)
            throw runtime_error("Kernel function '" + functionName + "' already added.");
    }

    functions.push_back(KernelFunction(functionName, globalSize, localSize));
}

const string &KernelFile::getSourceCode() const { return source_code; }
const string &KernelFile::getFileName() const { return file_name; }
const vector<KernelFunction>& KernelFile::getKernelFunctions() const { return functions; }

void KernelFile::showKernelFileInfo() const
{
    cout << "Kernel file: " << file_name << '\n';
    cout << "Contains " << functions.size() << " kernel function(s):\n";

    for (int i = 0; i < functions.size(); ++i)
        functions[i].showKernelFunctionInfo();
}