#include "common.h"

using namespace std;

enum class Access {
    ReadOnly,
    WriteOnly,
    ReadWrite
};

class Buffer : public Data {
    private:
        cl_mem mem = nullptr;
        size_t size;
        Access access;
        void* host_data = nullptr;

    public:
        Buffer(size_t count, size_t elementSize, Access access, void* hostPtr = nullptr);

        template<typename T>
        static Buffer fromValues(const vector<T>& data, Access access) {
            return Buffer(data.size(), sizeof(T), access, (void*)data.data());
        }

        template<typename T>
        static Buffer fromValues(initializer_list<T> data, Access access) {
            return Buffer(data.size(), sizeof(T), access, (void*)data.begin());
        }

        template<typename T>
        static Buffer empty(size_t count, Access access) {
            return Buffer(count, sizeof(T), access);
        }

        template<typename T>
        vector<T> readBack(cl_command_queue queue) {
            vector<T> result(size / sizeof(T));
            
            cl_event readEvent;

            cl_int err = clEnqueueReadBuffer(queue, mem, CL_TRUE, 0, size, result.data(), 0, nullptr, &readEvent);
            if (err != CL_SUCCESS)
                throw runtime_error("Failed to read buffer! Error code: " + to_string(err));

            clFinish(queue);

            cl_ulong start = 0, end = 0;
            clGetEventProfilingInfo(readEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, nullptr);
            clGetEventProfilingInfo(readEvent, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, nullptr);

            double elapsed_time_ms = (end - start) * 1e-6;
            cout << "Copy data from Buffer duration: " << elapsed_time_ms << " ms\n";

            clReleaseEvent(readEvent);
            
            return result;
        }

        void setKernelArguments(cl_kernel kernel, cl_uint index) const override;

        void create(cl_context context) override;

        ~Buffer();
};
