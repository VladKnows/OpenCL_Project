#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>
#include <iostream>
#include <vector>

#include "platform/platformDevices.h"

using namespace std;

PlatformDevices::PlatformDevices() = default;

PlatformDevices::PlatformDevices(cl_platform_id platform_id, const vector<Device>& selectedDevices)
{
    platform = platform_id;
    devices = selectedDevices;
    
    createContextAndQueues();
}

void PlatformDevices::showDevices()
{
    for(int i = 0; i < devices.size(); ++i)
    {
        cl_device_type type;
        clGetDeviceInfo(devices[i].id, CL_DEVICE_TYPE, sizeof(type), &type, nullptr);

        string type_str;
        if (type & CL_DEVICE_TYPE_CPU) type_str += "CPU";
        if (type & CL_DEVICE_TYPE_GPU) type_str += "GPU";
        if (type & CL_DEVICE_TYPE_ACCELERATOR) type_str += "Accelerator";
        if (type & CL_DEVICE_TYPE_DEFAULT) type_str += "Default";

        cout << "\tDevice #" << i << ": " << devices[i].name << " [" << type_str << "]\n";
    }
}

void PlatformDevices::createContextAndQueues()
{
    if (devices.empty()) {
        cerr << "No devices to create context and queues!\n";
        return;
    }

    vector<cl_device_id> device_ids;
    for (int i = 0; i < devices.size(); ++i)
        device_ids.push_back(devices[i].id);

    cl_int err;
    context = clCreateContext(nullptr, devices.size(), device_ids.data(), nullptr, nullptr, &err);

    if (err != CL_SUCCESS) {
        cerr << "Failed to create OpenCL context! Error: " << err << '\n';
        return;
    }

    cout << "Context created successfully.\n";

    for(auto& device : devices)
    {
        cl_command_queue_properties properties[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };

        device.queue = clCreateCommandQueueWithProperties(context, device.id, properties, &err);

        if (err != CL_SUCCESS)
            cerr << "Failed to create command queue! Error: " << err << '\n';
        else
            cout << "Command queue created for device.\n";
    }
}