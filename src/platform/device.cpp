#include "common.h"

Device::Device(cl_device_id id, const string &type, const string &name)
    : device_id(id), device_type(type), device_name(name) {}

const cl_device_id &Device::getDeviceId() const { return device_id; }
const string &Device::getDeviceType() const { return device_type; }
const string &Device::getDeviceName() const { return device_name; }
const cl_command_queue &Device::getCommandQueue() const { return command_queue; }

void Device::showDeviceInfo() const
{
    cout << "\tDevice #" << device_id
            << ": " << device_name
            << " [" << device_type << "]\n";
}

void Device::addCommandToQueue(cl_kernel kernel, cl_uint workDim, size_t *globalSize, size_t *localSize)
{
    if(command_queue == nullptr)
    {
        cerr << "Command queue wasn't created!\n";
        return;
    }

    if(globalSize == nullptr)
    {
        cerr << "Global size cannot be nullptr!\n";
        return;
    }

    if(kernel == nullptr)
    {
        cerr << "Kernel is nullptr!\n";
        return;
    }

    const size_t* local = nullptr;
    if(localSize != nullptr && *localSize > 0)
        local = localSize;

    cl_int err = clEnqueueNDRangeKernel(command_queue, kernel, workDim, nullptr, globalSize, local, 0, nullptr, nullptr);
    if (err != CL_SUCCESS)
        cerr << "Failed to enqueue kernel! Error: " << err << '\n';

    clFinish(command_queue);
}

void Device::createCommandQueue(cl_context context)
{
    cl_int err;
    cl_command_queue_properties properties[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
    command_queue = clCreateCommandQueueWithProperties(context, device_id, properties, &err);
    
    if (err != CL_SUCCESS)
        cerr << "Failed to create command queue for device " << device_name << ". Error: " << err << '\n';
    else
        cout << "Command queue created for device " << device_name << ".\n";
}

Device::~Device()
{
    if (command_queue != nullptr)
    {
        clReleaseCommandQueue(command_queue);
        command_queue = nullptr;
    }
}