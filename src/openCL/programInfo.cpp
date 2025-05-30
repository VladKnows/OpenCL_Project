#include "../../include/openCL/programInfo.h"
#include <iostream>

ProgramInfo::ProgramInfo(const string &name)
{
    this->name = name;
}

void ProgramInfo::addKernel(KernelInfo kernel)
{
    //!Maybe add check for existance
    kernels.push_back(kernel);
}

void ProgramInfo::addKernel()
{
    while(1)
    {
        string fileName, functionName;
        cout << "Adding new kernel function...\n File name: ";
        cin >> fileName;
        cout << "\nFunction name: ";
        cin >> functionName;
        cout << '\n';

        if(KernelInfo::exists(fileName, functionName))
        {
            unsigned int globalSize, localSize;
            cout << "\nGlobal size: ";
            cin >> globalSize;
            cout << "\nLocal size: ";
            cin >> localSize;

            try
            {
                kernels.push_back(KernelInfo(fileName, functionName, globalSize, localSize));
                cout << "Kernel was added successfully!\n";
                break;
            }
            catch(const exception &ex)
            {
                cerr << "Error adding kernel: " << ex.what() << '\n';
            }
        }
    }
}

void ProgramInfo::printInfo()
{
    cout << "Program name: " << name << "\nIt contains the following kernel functions:\n\n";

    for(int i = 0; i < kernels.size(); ++i) {
        cout << "Kernel " << i << '\n';
        kernels[i].printInfo();
    }
}