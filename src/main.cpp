#include "common.h"

using namespace std;

void execute_vector_addition_f32(WorkingGroup &workingGroup)
{
    cout << "Excuting: Vector addition...\n\n";

    // KernelFiles
    KernelFile vectorAdd("vector_add.cl");

    // Host memory initialization
    const int N = 1024;
    Utils::generateRandomMatrixToFile("vector_add_f32", "a.txt", N, 1);
    Utils::generateRandomMatrixToFile("vector_add_f32", "b.txt", N, 1);

    vector<float> a_vec = Utils::readVectorFromFile<float>("vector_add_f32", "a.txt");
    vector<float> b_vec = Utils::readVectorFromFile<float>("vector_add_f32", "b.txt");

    // KernelFunctions
    vectorAdd.addKernelFunction("vector_add", 1024);

    // Program initialization
    Program program("VectorOps");
    program.addKernelFunctions(vectorAdd);

    // OpenCL Data - Buffers and Scalar
    Buffer a = Buffer::fromValues<float>(a_vec, Access::ReadOnly);
    Buffer b = Buffer::fromValues<float>(b_vec, Access::ReadOnly);
    Buffer c = Buffer::empty<float>(N, Access::ReadWrite);
    Scalar n = Scalar::fromValue<unsigned int>(N);

    // KernelExecutor - program flow
    KernelExecutor executor(program);
    executor.addKernelCall("vector_add", {&a, &b, &c, &n});

    // Running
    workingGroup.runOnOneDevice(executor);

    // Result
    vector<float> result = c.readBack<float>(program.getDevices()[0]->getCommandQueue());
    Utils::writeVectorToFile<float>("vector_add_f32", "c.txt", result);
}

void execute_reduce_f32(WorkingGroup &workingGroup)
{
    cout << "Executing: Reduce (sum)...\n\n";

    // KernelFiles
    KernelFile reduce("reduce.cl");

    // Host memory initialization
    Utils::generateRandomMatrixToFile("reduce_f32", "in.txt", 4096, 1);
    vector<float> input = Utils::readVectorFromFile<float>("reduce_f32", "in.txt");
    const unsigned int N = input.size();

    unsigned int localSize = 256;
    unsigned int numGroups = (N + localSize - 1) / localSize;
    unsigned int globalSize = numGroups * localSize;

    // KernelFunctions
    reduce.addKernelFunction("reduce_f32_stage1", globalSize, localSize);
    reduce.addKernelFunction("reduce_f32_stage2", localSize, localSize);

    // Program initialization
    Program program("Reduce");
    program.addKernelFunctions(reduce);

    // OpenCL Data - Buffers and Scalar
    Buffer inputBuf = Buffer::fromValues<float>(input, Access::ReadOnly);
    Buffer partialBuf = Buffer::empty<float>(numGroups, Access::ReadWrite);
    Buffer outputBuf = Buffer::empty<float>(1, Access::ReadWrite);

    Scalar N_scalar = Scalar::fromValue<unsigned int>(N);
    Scalar G_scalar = Scalar::fromValue<unsigned int>(numGroups);

    // KernelExecutor - program flow
    KernelExecutor executor(program);
    executor.addKernelCall("reduce_f32_stage1", {&inputBuf, &partialBuf, &N_scalar});
    executor.addKernelCall("reduce_f32_stage2", {&partialBuf, &outputBuf, &G_scalar});

    // Running
    workingGroup.runOnOneDevice(executor);

    // Result
    vector<float> result = outputBuf.readBack<float>(program.getDevices()[0]->getCommandQueue());
    Utils::writeVectorToFile<float>("reduce_f32", "out.txt", result);
}

void execute_fir_f32(WorkingGroup &workingGroup)
{
    cout << "Excuting: FIR on 32 bits elements...\n\n";

    // KernelFiles
    KernelFile fir("fir.cl");

    // Host memory initialization
    Utils::generateRandomMatrixToFile("fir_f32", "in.txt", 384, 1);
    Utils::generateRandomMatrixToFile("fir_f32", "coeff.txt", 256, 1);

    vector<float> input = Utils::readVectorFromFile<float>("fir_f32", "in.txt");
    vector<float> coeffs = Utils::readVectorFromFile<float>("fir_f32", "coeff.txt");

    const unsigned int blockSize = input.size();
    const unsigned int numTaps = coeffs.size();

    vector<float> state(numTaps - 1, 0.0f);
    state.insert(state.end(), input.begin(), input.end());
    
    // KernelFunctions
    fir.addKernelFunction("fir_f32", blockSize);

    // Program initialization
    Program program("FirFilter");
    program.addKernelFunctions(fir);

    // OpenCL Data - Buffers and Scalar
    Buffer coeffsBuf = Buffer::fromValues<float>(coeffs, Access::ReadOnly);
    Buffer stateBuf = Buffer::fromValues<float>(state, Access::ReadOnly);
    Buffer outputBuf = Buffer::empty<float>(blockSize, Access::ReadWrite);

    Scalar numTapsScalar = Scalar::fromValue<unsigned int>(numTaps);
    Scalar blockSizeScalar = Scalar::fromValue<unsigned int>(blockSize);

    // KernelExecutor - program flow
    KernelExecutor executor(program);
    executor.addKernelCall("fir_f32", {&coeffsBuf, &stateBuf, &outputBuf, &numTapsScalar, &blockSizeScalar});

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
    Utils::generateRandomMatrixToFile("dct4_f32", "in.txt", 2048, 1);

    vector<float> input = Utils::readVectorFromFile<float>("dct4_f32", "in.txt");
    const unsigned int N = input.size();

    // KernelFunctions
    dct.addKernelFunction("dct4_f32", 2, {N, N}, {256, 1});

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

void execute_matrix_multiplication_f32(WorkingGroup &workingGroup)
{
    cout << "Executing: Matrix Multiplication on 32-bit float elements...\n\n";

    // KernelFiles
    KernelFile matmul("matrix_multiplication.cl");

    // Host memory initialization
    unsigned int M, N, P;
    unsigned int rowsA, colsA, rowsB, colsB;

    Utils::generateRandomMatrixToFile("matrix_multiplication_f32", "A.txt", 256, 512);
    Utils::generateRandomMatrixToFile("matrix_multiplication_f32", "B.txt", 512, 128);

    vector<float> A = Utils::readMatrixFromFile<float>("matrix_multiplication_f32", "A.txt", rowsA, colsA);
    vector<float> B = Utils::readMatrixFromFile<float>("matrix_multiplication_f32", "B.txt", rowsB, colsB);

    if (colsA != rowsB)
        throw runtime_error("Matrix dimensions incompatible for multiplication");

    M = rowsA;
    N = colsA;
    P = colsB;

    vector<float> C(M * P, 0.0f);

    // KernelFunctions
    matmul.addKernelFunction("matrix_multiplication_f32", 2, {M, P});

    // Program initialization
    Program program("MatrixMultiplication");
    program.addKernelFunctions(matmul);

    // OpenCL Data - Buffers and Scalars
    Buffer aBuf = Buffer::fromValues<float>(A, Access::ReadOnly);
    Buffer bBuf = Buffer::fromValues<float>(B, Access::ReadOnly);
    Buffer cBuf = Buffer::fromValues<float>(C, Access::ReadWrite);

    Scalar mScalar = Scalar::fromValue<unsigned int>(M);
    Scalar nScalar = Scalar::fromValue<unsigned int>(N);
    Scalar pScalar = Scalar::fromValue<unsigned int>(P);

    // KernelExecutor - program flow
    KernelExecutor executor(program);
    executor.addKernelCall("matrix_multiplication_f32", {&aBuf, &bBuf, &cBuf, &mScalar, &nScalar, &pScalar});

    // Running
    workingGroup.runOnOneDevice(executor);

    // Result
    vector<float> result = cBuf.readBack<float>(program.getDevices()[0]->getCommandQueue());

    // Optional output
    Utils::writeVectorToFile<float>("matrix_multiplication_f32", "out.txt", result);
}

int main()
{
    // WorkingGroup initialization
    WorkingGroup workingGroup(SelectionMode::Manual);
    workingGroup.showSelectedPlatformsDevices();

    // Executing user defined functions
    cout << "=====================================================\n";
    execute_vector_addition_f32(workingGroup);
    cout << "=====================================================\n";
    execute_reduce_f32(workingGroup);
    cout << "=====================================================\n";
    execute_fir_f32(workingGroup);
    cout << "=====================================================\n";
    execute_dct4_f32(workingGroup);
    cout << "=====================================================\n";
    execute_matrix_multiplication_f32(workingGroup);
    cout << "=====================================================\n";

    return 0;
}
