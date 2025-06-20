#include "common.h"
using namespace std;

struct KernelCall {
    string functionName;
    vector<Buffer*> arguments;
};

class KernelExecutor {
    private:
        Program &program;

        vector<KernelCall> executionList;
    
    public:
        KernelExecutor(Program &program);

        Program& getProgram();
        
        void addKernelCall(const string &functionName, const vector<Buffer*> &args);
        void allocateBuffers();
        void execute();
};