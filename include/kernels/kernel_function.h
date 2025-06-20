#include "common.h"
using namespace std;

class KernelFunction {
    private:
        const string functionName;
        const size_t globalSize;
        const size_t localSize;
        
    public:
        KernelFunction(const string &name, size_t global, size_t local = 0);

        const string &getFunctionName() const;

        void showKernelFunctionInfo() const;
};