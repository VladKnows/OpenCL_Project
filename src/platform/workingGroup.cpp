#include <CL/cl.h>
#include <iostream>
#include <vector>

#include "platform/workingGroup.h"

using namespace std;

WorkingGroup::WorkingGroup() {}

void WorkingGroup::selectPlatforms()
{
    platformDevices.clear();

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

    // !Remove dummies
    cl_platform_id dummy1 = (cl_platform_id)0x1111;
    cl_platform_id dummy2 = (cl_platform_id)0x2222;
    cl_platform_id dummy3 = (cl_platform_id)0x3333;

    platforms.push_back(dummy1);
    platforms.push_back(dummy2);
    platforms.push_back(dummy3);

    num_platforms += 3;
    // !Remove up to here

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
            
            // !Remove dummies
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
    
            cout << "\t[" << (selected[i] ? '*' : ' ') << "] Platform #" << i << ": " << name << '\n';
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
            no_selected_platforms += (selected[choice] ? -1 : 1);

            selected[choice] = !selected[choice];
        }
    }

    for (int i = 0; i < num_platforms; ++i) {
        if (selected[i]) {
            cl_uint num_devices = 0;
            cl_int err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
            if (err != CL_SUCCESS || num_devices == 0) {
                cerr << "No devices found for platform " << i << " or error: " << err << '\n';
                continue;
            }
    
            vector<cl_device_id> allDevices(num_devices);
            err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, num_devices, allDevices.data(), nullptr);
            if (err != CL_SUCCESS) {
                cerr << "Failed to get devices for platform " << i << ". Error: " << err << '\n';
                continue;
            }
            
            bool deviceSelected[num_devices];
            for(int i = 0; i < num_devices; ++i)
                selected[i] = 0;
            
            int selectedCount = 0;
            int devChoice = 0;
    
            while (true) {
                cout << "\nSelect/Deselect devices for Platform #" << i << ":\n";
                for (int j = 0; j < num_devices; ++j) {
                    size_t name_size = 0;
                    clGetDeviceInfo(allDevices[j], CL_DEVICE_NAME, 0, nullptr, &name_size);
                    vector<char> name_buf(name_size);
                    clGetDeviceInfo(allDevices[j], CL_DEVICE_NAME, name_size, name_buf.data(), nullptr);
    
                    cout << "\t[" << (deviceSelected[j] ? '*' : ' ') << "] Device #" << j << ": " << name_buf.data() << '\n';
                }
    
                cout << "\tOr enter " << num_devices << " to finish selection.\n";
                cout << "Enter number: ";
                cin >> devChoice;
    
                if (devChoice == num_devices) {
                    if (selectedCount == 0)
                        cout << "No devices selected!\n";
                    else
                        break;
                }
                else if (devChoice >= 0 && devChoice < num_devices) {
                    deviceSelected[devChoice] = !deviceSelected[devChoice];
                    selectedCount += deviceSelected[devChoice] ? 1 : -1;
                }
                else
                    cout << "Invalid choice.\n";
            }
    
            vector<cl_device_id> selectedDevices;
            for (int j = 0; j < num_devices; ++j)
                if (deviceSelected[j])
                    selectedDevices.push_back(allDevices[j]);

            platformDevices.push_back(PlatformDevices(platforms[i], selectedDevices));
        }
    }
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