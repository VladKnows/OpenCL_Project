#pragma once
#include <vector>
#include <CL/cl.h>

using namespace std;

// Class that wraps an OpenCL platform and its devices, context, annd command queues
class PlatformDevices {
    private:
        // Represents an OpenCL platform (Radxa ROCK 5C has: ARM Mali)
        // A platform can have multiple devices (Example: Laptop processors with integrated graphics)
        cl_platform_id platform;

        // List of OpenCL devices avalabile on that platform (ARM Mali on ROCK 5C has: GPU Mali)
        vector<cl_device_id> devices;

        // Context is used to manage and coordinate memory, kernels, and command queues acress one or more devices
        // !An improvement can be made by having multiple contexts
        cl_context context = nullptr;

        // A separate command queue for each device on the platform
        // Commands can be: kernel execution, memory operations, and synchronization
        vector<cl_command_queue> commandQueues;

        void createContextAndQueues();
    
    public:
        PlatformDevices();
        PlatformDevices(cl_platform_id platform_id);
        PlatformDevices(cl_platform_id platform_id, const vector<cl_device_id>& selectedDevices);
    
        cl_context getContext() const { return context; }
        const vector<cl_command_queue>& getQueues() const { return commandQueues; }

        void showDevices();
};