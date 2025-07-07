#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include "utils.h"
#include "opencl_data/data.h"
#include "opencl_data/scalar.h"
#include "opencl_data/buffer.h"
#include "platform/device.h"
#include "kernels/kernel_function.h"
#include "kernels/kernel_file.h"
#include "kernels/program.h"
#include "kernels/kernel_executor.h"
#include "platform/platform.h"
#include "platform/working_group.h"