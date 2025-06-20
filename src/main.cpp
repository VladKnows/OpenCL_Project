#include "common.h"

using namespace std;

int main()
{
    //WorkingGroup initialization
    WorkingGroup workingGroup(SelectionMode::Automatic);
    workingGroup.showSelectedPlatformsDevices();

    //KernelFiles and KernelFunctions
    KernelFile vectorAdd("vector_add.cl");
    vectorAdd.addKernelFunction("vector_add", 1024, 64);

    //Program initialization
    Program program("VectorOps");
    program.addKernelFunctions(vectorAdd);

    //KernelExecutor - program flow
    KernelExecutor executor(program);

    const int N = 1024;
    Buffer a = Buffer::fromValues<float>(vector<float>(N, 1.0f), Access::ReadOnly);
    Buffer b = Buffer::fromValues<float>(vector<float>(N, 2.0f), Access::ReadOnly);
    Buffer c = Buffer::empty<float>(N, Access::ReadWrite);
    Buffer n = Buffer::fromValues<unsigned int>({static_cast<unsigned int>(N)}, Access::ReadOnly);
    executor.addKernelCall("vector_add", {&a, &b, &c, &n});

    //Running
    workingGroup.runOnOneDevice(executor);

    //Result
    vector<float> result = c.readBack<float>(program.getDevices()[0]->getCommandQueue());
    cout << "Result: ";
    for (int i = 0; i < 10; ++i)
        cout << result[i] << ' ';
    cout << '\n';

    return 0;
}
