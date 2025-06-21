#include "common.h"

using namespace std;

class KernelFile {
    private:
        const string file_name;
        vector<KernelFunction> functions;
        string source_code;

        bool kernelFileExists() const;
        bool kernelFunctionExists(const string &function) const;
        void readSourceCode();

    public:
        KernelFile(const string &file);

        void addKernelFunction(const string &functionName, size_t globalSize, size_t localSize = 0);

        const string &getSourceCode() const;
        const string &getFileName() const;
        const vector<KernelFunction>& getKernelFunctions() const;

        void showKernelFileInfo() const;
};