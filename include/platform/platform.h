#include "common.h"

using namespace std;

class Platform {
    private:
        const cl_platform_id platform_id;
        const string platform_name;
        vector<Device> devices;
        cl_context context = nullptr;
    
    public:
        Platform(cl_platform_id id, const string &name, const vector<Device>& selectedDevices);

        const cl_platform_id &getPlatformId() const;
        const string &getPlatformName() const;
        const cl_context &getContext() const;
        const vector<Device> &getDevices() const;

        void showPlatformInfo() const;
        void showSelectedDevices() const;

        void createContextAndQueues();
        void addCommandToDevice(int deviceIndex, cl_kernel kernel, cl_uint workDim, size_t* globalSize, size_t* localSize = nullptr);

        ~Platform();
};