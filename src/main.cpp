#include "common.h"

using namespace std;

void execute_2vector_additions(WorkingGroup &workingGroup)
{
    cout << "Excuting: 2 vector additions...\n\n";

    // KernelFiles
    KernelFile vectorAdd("vector_add.cl");

    // Host memory initialization
    const int N = 1024;
    vector<float> a_vec(N, 1.0f);
    vector<float> b_vec(N, 2.0f);
    vector<float> d_vec(N, 5.0f);

    // KernelFunctions
    vectorAdd.addKernelFunction("vector_add", 1024, 64);

    // Program initialization
    Program program("VectorOps");
    program.addKernelFunctions(vectorAdd);

    // OpenCL Data - Buffers and Scalar
    Buffer a = Buffer::fromValues<float>(a_vec, Access::ReadOnly);
    Buffer b = Buffer::fromValues<float>(b_vec, Access::ReadOnly);
    Buffer c = Buffer::empty<float>(N, Access::ReadWrite);
    Buffer d = Buffer::fromValues<float>(d_vec, Access::ReadOnly);
    Scalar n = Scalar::fromValue<unsigned int>(N);

    // KernelExecutor - program flow
    KernelExecutor executor(program);
    executor.addKernelCall("vector_add", {&a, &b, &c, &n});
    executor.addKernelCall("vector_add", {&c, &d, &c, &n});

    // Running
    workingGroup.runOnOneDevice(executor);

    // Result
    vector<float> result = c.readBack<float>(program.getDevices()[0]->getCommandQueue());
}

void execute_fir_f32(WorkingGroup &workingGroup)
{
    cout << "Excuting: FIR on 32 bits elements...\n\n";

    // KernelFiles
    KernelFile fir("fir.cl");

    // Host memory initialization
    vector<float> input = Utils::readVectorFromFile<float>("fir_f32", "in.txt");
    vector<float> coeffs = Utils::readVectorFromFile<float>("fir_f32", "coeff.txt");

    const unsigned int blockSize = input.size();
    const unsigned int numTaps = coeffs.size();

    vector<float> state(numTaps - 1, 0.0f);
    state.insert(state.end(), input.begin(), input.end());
    
    // KernelFunctions
    fir.addKernelFunction("fir_f32", blockSize, 0);

    // Program initialization
    Program program("FirFilter");
    program.addKernelFunctions(fir);

    // OpenCL Data - Buffers and Scalar
    Buffer inputBuf = Buffer::fromValues<float>(input, Access::ReadOnly);
    Buffer coeffsBuf = Buffer::fromValues<float>(coeffs, Access::ReadOnly);
    Buffer stateBuf = Buffer::fromValues<float>(state, Access::ReadOnly);
    Buffer outputBuf = Buffer::empty<float>(blockSize, Access::ReadWrite);

    Scalar numTapsScalar = Scalar::fromValue<unsigned int>(numTaps);
    Scalar blockSizeScalar = Scalar::fromValue<unsigned int>(blockSize);

    // KernelExecutor - program flow
    KernelExecutor executor(program);
    executor.addKernelCall("fir_f32", {&inputBuf, &coeffsBuf, &stateBuf, &outputBuf, &numTapsScalar, &blockSizeScalar});

    // Running
    workingGroup.runOnOneDevice(executor);

    // Result
    vector<float> result = outputBuf.readBack<float>(program.getDevices()[0]->getCommandQueue());
    Utils::writeVectorToFile<float>("fir_f32", "out.txt", result);
}

void execute_dct4_f32(WorkingGroup &workingGroup)
{
    cout << "Executing: DCT4 on 32-bit float elements...\n\n";

    // KernelFiles
    KernelFile dct("dct4.cl");

    // Host memory initialization
    vector<float> input = Utils::readVectorFromFile<float>("dct4_f32", "in.txt");
    const unsigned int N = input.size();

    // KernelFunctions
    dct.addKernelFunction("dct4_f32", N, 0);

    // Program initialization
    Program program("DCT4Transform");
    program.addKernelFunctions(dct);

    // OpenCL Data - Buffers and Scalar
    Buffer inputBuf = Buffer::fromValues<float>(input, Access::ReadOnly);
    Buffer outputBuf = Buffer::empty<float>(N, Access::ReadWrite);
    Scalar sizeScalar = Scalar::fromValue<unsigned int>(N);

    // KernelExecutor - program flow
    KernelExecutor executor(program);
    executor.addKernelCall("dct4_f32", {&inputBuf, &outputBuf, &sizeScalar});

    // Running
    workingGroup.runOnOneDevice(executor);

    // Result
    vector<float> result = outputBuf.readBack<float>(program.getDevices()[0]->getCommandQueue());
    Utils::writeVectorToFile<float>("dct4_f32", "out.txt", result);
}

int main()
{
    //WorkingGroup initialization
    WorkingGroup workingGroup(SelectionMode::Manual);
    workingGroup.showSelectedPlatformsDevices();

    //Executing user defined functions
    cout << "======================================================================\n";
    execute_2vector_additions(workingGroup);
    cout << "======================================================================\n";
    execute_fir_f32(workingGroup);
    cout << "======================================================================\n";
    execute_dct4_f32(workingGroup);
    cout << "======================================================================\n";

    return 0;
}
