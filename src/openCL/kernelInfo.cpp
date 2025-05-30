#include "../../include/openCL/kernelInfo.h"
#include <fstream>
#include <sstream>
#include <iostream>

KernelInfo::KernelInfo(const string &fileName, const string &functionName, unsigned int globalSize, unsigned int localSize)
{
    this->fileName = fileName;
    this->functionName = functionName;
    this->globalSize = globalSize;
    this->localSize = localSize;
}

void KernelInfo::printInfo()
{
    cout << "Kernel file name: " << fileName
        << "\nKernel function name: " << functionName
        << "\nGlobal size: " << globalSize
        << "\nLocal size: " << localSize << "\n\n";
}

bool KernelInfo::exists(const string &fileName, const string &functionName)
{
    string fullPath = "kernels/" + fileName;
    ifstream file(fullPath);

    if(!file.is_open())
    {
        cerr << "Cannot open file: " << fullPath << '\n';
        return 0;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();

    string search = "__kernel void " + functionName;
    if(source.find(search) != string::npos)
    {
        cout << "Kernel: " << functionName << " from file: " << fileName << " exists\n";
        return 1;
    }
    else
    {
        cout << "Kernel: " << functionName << " isn't present in file: " << fileName << '\n';
        return 0;
    }
}