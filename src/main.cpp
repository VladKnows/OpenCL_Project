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
    vector<float> a_vec(N, 1.0f);
    vector<float> b_vec(N, 2.0f);
    Buffer a = Buffer::fromValues<float>(a_vec, Access::ReadWrite);
    Buffer b = Buffer::fromValues<float>(b_vec, Access::ReadWrite);
    Buffer c = Buffer::empty<float>(N, Access::ReadWrite);
    Buffer n = Buffer::fromValues<unsigned int>({static_cast<unsigned int>(N)}, Access::ReadOnly);
    executor.addKernelCall("vector_add", {&a, &b, &c, &n});

    //Running
    workingGroup.runOnOneDevice(executor);

    //Result
    auto a_vals = a.readBack<float>(program.getDevices()[0]->getCommandQueue());
    auto b_vals = b.readBack<float>(program.getDevices()[0]->getCommandQueue());
    for (int i = 0; i < 5; ++i)
        cout << "a[" << i << "] = " << a_vals[i] << ", b[" << i << "] = " << b_vals[i] << endl;
    
    vector<float> result = c.readBack<float>(program.getDevices()[0]->getCommandQueue());
    cout << "Result: ";
    for (int i = 0; i < 1024; ++i)
        cout << result[i] << ' ';
    cout << '\n';

    return 0;
}
