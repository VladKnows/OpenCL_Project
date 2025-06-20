#include "common.h"

using namespace std;

enum class Access {
    ReadOnly,
    WriteOnly,
    ReadWrite
};

class Buffer {
private:
    cl_mem mem = nullptr;
    size_t size;
    Access access;
    void* hostData = nullptr;

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

        cl_int err = clEnqueueReadBuffer(queue, mem, CL_TRUE, 0, size, result.data(), 0, nullptr, nullptr);
        if (err != CL_SUCCESS)
            throw runtime_error("Failed to read buffer! Error code: " + to_string(err));
        return result;
    }

    void create(cl_context context);

    cl_mem getCLBuffer(const cl_context &context) const;

    ~Buffer();
};
