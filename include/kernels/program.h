#include "common.h"

using namespace std;

class Program {
    private:
        const string program_name;

        cl_context context;
        vector<Device*> devices;
        cl_program program = nullptr;

        vector<KernelFile> kernel_files;
        vector<KernelFunction> kernel_functions;

    public:
        Program(const string &name, cl_context context, const vector<Device*> &devices);
        Program(const string &name);

        void addKernelFunctions(const KernelFile &file);
        void compileProgram();

        void setContext(cl_context context);
        void setDevices(const vector<Device*> &devs);

        const string &getProgramName() const;
        cl_context getContext() const;
        cl_program getProgram() const;
        const vector<Device*>& getDevices() const;
        const vector<KernelFunction> &getKernelFunctions() const;

        void showProgramInfo() const;

        ~Program();
};