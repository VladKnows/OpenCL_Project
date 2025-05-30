#include <vector>
#include "kernelInfo.h"

using namespace std;

class ProgramInfo {
    private:
        string name;
        vector<KernelInfo> kernels;

    public:
        ProgramInfo(const string &name);

        void addKernel();
        void addKernel(KernelInfo kernel);
        void printInfo();
};