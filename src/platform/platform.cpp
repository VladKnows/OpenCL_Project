#include "common.h"

using namespace std;

Platform::Platform(cl_platform_id id, const string &name, const vector<Device>& selectedDevices)
    : platform_id(id), platform_name(name), devices(selectedDevices) {}

const cl_platform_id &Platform::getPlatformId() const { return platform_id; }
const string &Platform::getPlatformName() const { return platform_name; }
const cl_context &Platform::getContext() const { return context; }
const vector<Device> &Platform::getDevices() const { return devices; }

void Platform::createContextAndQueues()
{
    if (devices.empty())
        throw runtime_error("No devices to create context and queues!\n");

    vector<cl_device_id> deviceIds;
    for (int i = 0; i < devices.size(); ++i)
        deviceIds.push_back(devices[i].getDeviceId());

    cl_int err;
    context = clCreateContext(nullptr, devices.size(), deviceIds.data(), nullptr, nullptr, &err);

    if (err != CL_SUCCESS)
    {
        throw runtime_error("Failed to create OpenCL context! Error: " + to_string(err) + '\n');
        return;
    }
    cout << "Context created successfully.\n";

    for (int i = 0; i < devices.size(); ++i)
    {
        cout << "Creating command queue for device: " << devices[i].getDeviceName() << '\n';
        devices[i].createCommandQueue(context);
    }
}

void Platform::showPlatformInfo() const
{
    cout << "Platform #" << platform_id << ": " << platform_name << '\n';
}

void Platform::showSelectedDevices() const
{
    for (int i = 0; i < devices.size(); ++i)
        devices[i].showDeviceInfo();
}

void Platform::addCommandToDevice(int deviceIndex, cl_kernel kernel, cl_uint workDim, size_t* globalSize, size_t* localSize)
{
    if (deviceIndex < 0 || deviceIndex >= devices.size())
        throw runtime_error("Invalid device index!\n");

    devices[deviceIndex].addCommandToQueue(kernel, workDim, globalSize, localSize);
}

Platform::~Platform() {
    if (context != nullptr)
    {
        clReleaseContext(context);
        context = nullptr;
    }
}