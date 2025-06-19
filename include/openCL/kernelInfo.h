#include "common.h"
using namespace std;

class KernelInfo {
    private:
        string fileName;
        string functionName;
        unsigned int globalSize;
        unsigned int localSize;

    public:
        KernelInfo(const string &fileName, const string &functionName, unsigned int globalSize, unsigned int localSize);

        void printInfo();
        
        static bool exists(const string &fileName, const string &functionName);
};