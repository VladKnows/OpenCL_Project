#pragma once
#include <vector>
#include <CL/cl.h>

#include "platform/platformDevices.h"

using namespace std;

class WorkingGroup {
    private:
        vector<PlatformDevices> platformDevices;

        void addPlatformDevices(const PlatformDevices& platformDevice);

    public:
        WorkingGroup();

        void showDevices();
};