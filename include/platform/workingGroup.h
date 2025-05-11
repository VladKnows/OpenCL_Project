#pragma once
#include <vector>
#include <CL/cl.h>

#include "platform/platformDevices.h"

using namespace std;

// Class for managing multiple PlattformDevices instances, each corresponding to a selected platform
class WorkingGroup {
    private:
        vector<PlatformDevices> platformDevices;

    public:
        WorkingGroup();
        
        // UI for selecting the desired platforms and their devices
        void selectPlatforms();
        void showDevices();
};