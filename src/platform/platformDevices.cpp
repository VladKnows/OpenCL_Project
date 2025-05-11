#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>
#include <iostream>
#include <vector>

#include "platform/platformDevices.h"

using namespace std;

PlatformDevices::PlatformDevices() = default;

PlatformDevices::PlatformDevices(cl_platform_id platform_id) : platform(platform_id) {
    cl_uint num_devices = 0;
    cl_int err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
    if (err != CL_SUCCESS || num_devices == 0) {
        cerr << "Warning: no devices found or error when querying devices! Error: " << err << '\n';
        return;
    }

    devices.resize(num_devices);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, devices.data(), nullptr);
    if (err != CL_SUCCESS) {
        cerr << "Error getting device IDs after resize! Error: " << err << '\n';
        devices.clear();
    }

    createContextAndQueues();
}

void PlatformDevices::showDevices()
{
    for(int i = 0; i < devices.size(); ++i)
    {
        size_t name_size = 0;
        clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 0, nullptr, &name_size);
        vector<char> name(name_size);
        clGetDeviceInfo(devices[i], CL_DEVICE_NAME, name_size, name.data(), nullptr);

        cl_device_type type;
        clGetDeviceInfo(devices[i], CL_DEVICE_TYPE, sizeof(type), &type, nullptr);

        string type_str;
        if (type & CL_DEVICE_TYPE_CPU) type_str += "CPU ";
        if (type & CL_DEVICE_TYPE_GPU) type_str += "GPU ";
        if (type & CL_DEVICE_TYPE_ACCELERATOR) type_str += "Accelerator ";
        if (type & CL_DEVICE_TYPE_DEFAULT) type_str += "Default ";

        cout << "\tDevice #" << i << ": " << name.data() << " [" << type_str << "]\n";
    }
}

void PlatformDevices::createContextAndQueues()
{
    if (devices.empty()) {
        cerr << "No devices to create context and queues!\n";
        return;
    }

    cl_int err;
    context = clCreateContext(nullptr, devices.size(), devices.data(), nullptr, nullptr, &err);

    if (err != CL_SUCCESS) {
        cerr << "Failed to create OpenCL context! Error: " << err << '\n';
        return;
    }

    cout << "Context created successfully.\n";
    commandQueues.clear();

    for(auto device : devices)
    {
        cl_command_queue_properties properties[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };

        cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, properties, &err);

        if (err != CL_SUCCESS) {
            cerr << "Failed to create command queue! Error: " << err << '\n';
        } else {
            commandQueues.push_back(queue);
            cout << "Command queue created for device.\n";
        }
    }
}