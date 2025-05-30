#pragma once
#include <vector>
#include <CL/cl.h>

using namespace std;

struct Device {
    // Device's ID in a platform
    cl_device_id id;

    // Name of the Device
    string name;

    // Command queue for a device
    // Commands can be: kernel execution, memory operations, and synchronization
    cl_command_queue queue;
};

// Class that wraps an OpenCL platform and its devices, context, annd command queues
class PlatformDevices {
    private:
        // Represents an OpenCL platform (Radxa ROCK 5C has: ARM Mali)
        // A platform can have multiple devices (Example: Laptop processors with integrated graphics)
        cl_platform_id platform;

        // List of OpenCL devices avalabile on that platform (ARM Mali on ROCK 5C has: GPU Mali)
        vector<Device> devices;

        // Context is used to manage and coordinate memory, kernels, and command queues acress one or more devices
        // !An improvement can be made by having multiple contexts
        cl_context context = nullptr;

        void createContextAndQueues();
    
    public:
        PlatformDevices();
        PlatformDevices(cl_platform_id platform_id, const vector<Device>& selectedDevices);
    
        cl_context getContext() { return context; }
        vector<Device> getDevices() const { return devices; }

        void showDevices();
};