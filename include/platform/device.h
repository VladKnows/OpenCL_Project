#include "common.h"

using namespace std;

class Device {
    private:
        const cl_device_id device_id;
        const string device_type;
        const string device_name;
        cl_command_queue command_queue = nullptr;

    public:
        Device(cl_device_id id, const string &type, const string &name);

        const cl_device_id &getDeviceId() const;
        const string &getDeviceType() const;
        const string &getDeviceName() const;

        void showDeviceInfo() const;
        
        // localSize value nullptr means auto
        void addCommandToQueue(cl_kernel kernel, cl_uint workDim, size_t *globalSize, size_t *localSize = nullptr);
        void createCommandQueue(cl_context context);

        ~Device();
};