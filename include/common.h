#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include "openCL/kernelInfo.h"
#include "openCL/programInfo.h"
#include "platform/device.h"
#include "platform/platform.h"
#include "platform/workingGroup.h"