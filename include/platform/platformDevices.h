#pragma once
#include <vector>
#include <CL/cl.h>

using namespace std;

class PlatformDevices {
    private:

        cl_platform_id platform;
        vector<cl_device_id> devices;
        cl_context context = nullptr;
        vector<cl_command_queue> commandQueues;

        void createContextAndQueues();
    
    public:
        PlatformDevices();
        PlatformDevices(cl_platform_id platform_id);
    
        cl_context getContext() const { return context; }
        const vector<cl_command_queue>& getQueues() const { return commandQueues; }

        void showDevices();

};