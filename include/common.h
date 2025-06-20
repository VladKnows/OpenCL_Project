#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include "kernels/kernel_function.h"
#include "kernels/kernel_file.h"
#include "kernels/program.h"
#include "platform/device.h"
#include "platform/platform.h"
#include "platform/working_group.h"