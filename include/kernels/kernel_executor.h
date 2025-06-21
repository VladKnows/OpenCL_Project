#include "common.h"

using namespace std;

struct KernelCall {
    string functionName;
    vector<Data*> arguments;
};

class KernelExecutor {
    private:
        Program &program;

        vector<KernelCall> execution_list;
    
    public:
        KernelExecutor(Program &program);

        Program& getProgram();
        
        void addKernelCall(const string &functionName, const vector<Data*> &args);
        void allocateBuffers();
        void execute();
};