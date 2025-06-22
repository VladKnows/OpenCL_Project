#include "common.h"

using namespace std;

class KernelFunction {
    private:
        const string function_name;
        const vector<size_t> global_size;
        const vector<size_t> local_size;
        const unsigned int workDim = 1;
        
    public:
        KernelFunction(const string &name, unsigned int workDim, const vector<size_t> &globalSizes, const vector<size_t> &localSizes = {});
        KernelFunction(const string &name, size_t globalSize, size_t localSize = 0);

        const string &getFunctionName() const;
        const vector<size_t>& getGlobalSize() const;
        const vector<size_t>& getLocalSize() const;
        const unsigned int &getWorkDim() const;

        void showKernelFunctionInfo() const;
};