#define CL_TARGET_OPENCL_VERSION 300

#include <vector>
#include <iostream>

#include "platform/platformDevices.h"
#include "platform/workingGroup.h"
#include <CL/cl.h>

using namespace std;

int main() {
    WorkingGroup workingGroup;
    workingGroup.selectPlatforms();
    workingGroup.showDevices();

    return 0;
}
