#include "common.h"

KernelFile::KernelFile(const string &file) : fileName(file)
{
    if (!kernelFileExists())
        throw runtime_error("Kernel file does not exist: " + fileName);

    readSourceCode();
}

bool KernelFile::kernelFileExists() const
{
    ifstream file("kernels/" + fileName);
    return file.good();
}

void KernelFile::readSourceCode()
{
    ifstream file("kernels/" + fileName);
    if (!file.is_open())
        throw runtime_error("Failed to open kernel source file: " + fileName);

    stringstream buffer;
    buffer << file.rdbuf();
    sourceCode = buffer.str();
}

bool KernelFile::kernelFunctionExists(const string &function) const
{
    string search = "__kernel void " + function;
    return sourceCode.find(search) != string::npos;
}

void KernelFile::addKernelFunction(const string &functionName, size_t globalSize, size_t localSize)
{
    if (!kernelFunctionExists(functionName))
    {
        cerr << "Kernel function '" << functionName << "' not found in file '" << fileName << "'\n";
        return;
    }

    for (int i = 0; i < functions.size(); ++i) {
        if (functions[i].getFunctionName() == functionName)
        {
            cerr << "Kernel function '" << functionName << "' already added.\n";
            return;
        }
    }

    functions.push_back(KernelFunction(functionName, globalSize, localSize));
    cout << "Kernel function '" << functionName << "' added successfully.\n";
}

const string &KernelFile::getSourceCode() const { return sourceCode; }
const string &KernelFile::getFileName() const { return fileName; }
const vector<KernelFunction>& KernelFile::getKernelFunctions() const { return functions; }

void KernelFile::showKernelFileInfo() const
{
    cout << "Kernel file: " << fileName << '\n';
    cout << "Contains " << functions.size() << " kernel function(s):\n";

    for(int i = 0; i < functions.size(); ++i)
        functions[i].showKernelFunctionInfo();
}