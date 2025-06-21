#include "common.h"

using namespace std;

class Scalar : public Data {
    private:
        void* data;
        size_t size;

    public:
        Scalar(void* valuePtr, size_t size);
            
        template<typename T>
        static Scalar fromValue(T value)
        {
            T* ptr = new T(value);
            return Scalar((void*)ptr, sizeof(T));
        }
        
        void setKernelArguments(cl_kernel kernel, cl_uint index) const override;

        void create(cl_context context) override;
};