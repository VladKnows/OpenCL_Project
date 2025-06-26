#include "common.h"

using namespace std;

class Data {
    public:
        virtual void setKernelArguments(cl_kernel kernel, cl_uint index) const = 0;

        virtual void create(cl_context context) = 0;

        virtual ~Data() = default;
};