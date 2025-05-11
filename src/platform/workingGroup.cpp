#include <CL/cl.h>
#include <iostream>
#include <vector>

#include "platform/workingGroup.h"

using namespace std;

WorkingGroup::WorkingGroup()
{
    cl_uint num_platforms = 0;
    cl_int err = clGetPlatformIDs(0, nullptr, &num_platforms);
    if (err != CL_SUCCESS) {
        cerr << "Failed to get number of platforms. Error: " << err << '\n';
        return;
    }

    cout << "Number of avalabile OpenCL platforms: " << num_platforms << '\n';

    if (num_platforms == 0) {
        cout << "No OpenCL platforms on this system!" << '\n';
        return;
    }

    vector<cl_platform_id> platforms(num_platforms);
    err = clGetPlatformIDs(num_platforms, platforms.data(), nullptr);
    if (err != CL_SUCCESS) {
        cerr << "Failed to get platform IDs. Error: " << err << '\n';
        return;
    }

    cl_platform_id dummy1 = (cl_platform_id)0x1111;
    cl_platform_id dummy2 = (cl_platform_id)0x2222;
    cl_platform_id dummy3 = (cl_platform_id)0x3333;

    platforms.push_back(dummy1);
    platforms.push_back(dummy2);
    platforms.push_back(dummy3);

    num_platforms += 3;

    bool selected[num_platforms];
    for(int i = 0; i < num_platforms; ++i)
        selected[i] = 0;
    int no_selected_platforms = 0;

    int choice = 0;
    while(1)
    {
        cout << "Select/Deselect platforms index (0 - " << num_platforms - 1 << "):\n";

        for (cl_uint i = 0; i < num_platforms; ++i) {
            string name;
            
            if (platforms[i] == dummy1) name = "Dummy Platform 1";
            else if (platforms[i] == dummy2) name = "Dummy Platform 2";
            else if (platforms[i] == dummy3) name = "Dummy Platform 3";
            else {
                size_t name_size = 0;
                clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, nullptr, &name_size);

                vector<char> name_buf(name_size);
                clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, name_size, name_buf.data(), nullptr);

                name = name_buf.data();
            }
    
            cout << "\t [";
            if(selected[i] == 0)
                cout << ' ';
            else
                cout << '*';
            cout << "] Platform #" << i << ": " << name.data() << '\n';
        }

        cout << "\tOr enter " << num_platforms << " to finish.\n\n";
        cout << "Enter number: ";

        while(1)
        {
            cin >> choice;
            
            if(choice < 0 || choice > num_platforms)
            {
                cout << "Number is not in the specified interval.\n";
                cout << "Enter another number: ";
            }
            else
                break;
        }

        if(choice == num_platforms)
        {
            if(no_selected_platforms == 0)
                cout << "No selected platforms!\n";
            else
                break;
        }
        else
        {
            if(selected[choice] == 0)
                no_selected_platforms++;
            else
                no_selected_platforms--;

            selected[choice] = !selected[choice];
        }
    }

    for(int i = 0; i < num_platforms; ++i)
    {
        if(selected[i] == 1)
            addPlatformDevices(PlatformDevices(platforms[i]));
    }
}

void WorkingGroup::addPlatformDevices(const PlatformDevices& platformDevice)
{
    platformDevices.push_back(platformDevice);
}

void WorkingGroup::showDevices()
{
    cout << '\n';
    for (int i = 0; i < platformDevices.size(); ++i) {
        cout << "Devices for Platform #" << i << ":\n";

        platformDevices[i].showDevices();
        cout << '\n';
    }
}