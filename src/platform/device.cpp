#include "common.h"

using namespace std;

#define CL_DEVICE_PCI_BUS_ID_NV 0x4008
#define CL_DEVICE_PCI_SLOT_ID_NV 0x4009

Device::Device(cl_device_id id, const string &type, const string &name)
    : device_id(id), device_type(type), device_name(name) {}

const cl_device_id &Device::getDeviceId() const { return device_id; }
const string &Device::getDeviceType() const { return device_type; }
const string &Device::getDeviceName() const { return device_name; }
const cl_command_queue &Device::getCommandQueue() const { return command_queue; }

void Device::showDeviceInfo() const
{
    cout << "\tDevice: " << device_name
            << " [" << device_type << "]\n";

    size_t max_work_group_size;
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_work_group_size, nullptr);

    cl_uint compute_units;
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &compute_units, nullptr);

    size_t max_work_item_sizes[3];
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(max_work_item_sizes), max_work_item_sizes, nullptr);

    cl_bool image_support;
    clGetDeviceInfo(device_id, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &image_support, nullptr);    

    cout << "\tCompute Units: " << compute_units << '\n';
    cout << "\tMax Work-Group Size: " << max_work_group_size << '\n';
    cout << "\tMax Work-Item Sizes: [" << max_work_item_sizes[0] << ", " << max_work_item_sizes[1] << ", " << max_work_item_sizes[2] << "]\n";

    cout << "\tImage Support: ";
    if (image_support)
        cout << "Yes";
    else
        cout << "No";
    cout << '\n';
}

void Device::addCommandToQueue(cl_kernel kernel, cl_uint workDim, size_t *globalSize, size_t *localSize)
{
    if (command_queue == nullptr)
    {
        throw runtime_error("Command queue wasn't created!\n");
        return;
    }

    if (globalSize == nullptr)
    {
        throw runtime_error("Global size cannot be nullptr!\n");
        return;
    }

    if (kernel == nullptr)
    {
        throw runtime_error("Kernel is nullptr!\n");
        return;
    }

    const size_t* local = nullptr;
    if (localSize != nullptr && *localSize > 0)
        local = localSize;

    cl_event writeEvent;
    cl_int err = clEnqueueNDRangeKernel(command_queue, kernel, workDim, nullptr, globalSize, local, 0, nullptr, &writeEvent);
    if (err != CL_SUCCESS)
        throw runtime_error("Failed to enqueue kernel! Error: " + to_string(err) + '\n');

    clFinish(command_queue);

    cl_ulong start = 0, end = 0;
    clGetEventProfilingInfo(writeEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, nullptr);
    clGetEventProfilingInfo(writeEvent, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, nullptr);

    double elapsed_time_ms = (end - start) * 1e-6;
    cout << "Execution time: " << elapsed_time_ms << " ms\n\n";

    clReleaseEvent(writeEvent);
}

void Device::createCommandQueue(cl_context context)
{
    cl_int err;
    cl_command_queue_properties properties[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
    command_queue = clCreateCommandQueueWithProperties(context, device_id, properties, &err);
    
    if (err != CL_SUCCESS)
        throw runtime_error("Failed to create command queue for device " + device_name + ". Error: " + to_string(err));
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