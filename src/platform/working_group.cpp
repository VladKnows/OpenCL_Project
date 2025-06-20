#include "common.h"

using namespace std;

WorkingGroup::WorkingGroup(SelectionMode selectionMode)
{
    if(selectionMode == SelectionMode::Automatic)
        automaticallySelectPlatforms();
    else
        manuallySelectPlatforms();

    createContextAndQueues();
}

void WorkingGroup::manuallySelectPlatforms()
{
    platforms.clear();

    cl_uint numPlatforms = 0;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    if (err != CL_SUCCESS)
    {
        cerr << "Failed to get number of platforms. Error: " << err << '\n';
        return;
    }

    cout << "Number of avalabile OpenCL platforms: " << numPlatforms << '\n';
    if (numPlatforms == 0)
    {
        cout << "No OpenCL platforms on this system!" << '\n';
        return;
    }

    vector<cl_platform_id> platformsIds(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platformsIds.data(), nullptr);
    if (err != CL_SUCCESS)
    {
        cerr << "Failed to get platform IDs. Error: " << err << '\n';
        return;
    }

    bool selected[numPlatforms];
    for(int i = 0; i < numPlatforms; ++i)
        selected[i] = 0;
    int nrOfSelectedPlatforms = 0;

    int choice = 0;
    while(1)
    {
        cout << "Select/Deselect platforms index (0 - " << numPlatforms - 1 << "):\n";

        for (cl_uint i = 0; i < numPlatforms; ++i) {
            string name;
            
            size_t nameSize = 0;
            clGetPlatformInfo(platformsIds[i], CL_PLATFORM_NAME, 0, nullptr, &nameSize);

            vector<char> nameBuffer(nameSize);
            clGetPlatformInfo(platformsIds[i], CL_PLATFORM_NAME, nameSize, nameBuffer.data(), nullptr);

            name = nameBuffer.data();
    
            cout << "\t[" << (selected[i] ? '*' : ' ') << "] Platform #" << i << ": " << name << '\n';
        }

        cout << "\tOr enter " << numPlatforms << " to finish.\n\n";
        cout << "Enter number: ";

        while(1)
        {
            cin >> choice;
            
            if(choice < 0 || choice > numPlatforms)
            {
                cout << "Number is not in the specified interval.\n";
                cout << "Enter another number: ";
            }
            else
                break;
        }

        if(choice == numPlatforms)
        {
            if(nrOfSelectedPlatforms == 0)
                cout << "No selected platforms!\n";
            else
                break;
        }
        else
        {
            nrOfSelectedPlatforms += (selected[choice] ? -1 : 1);
            selected[choice] = !selected[choice];
        }
    }

    for (int i = 0; i < numPlatforms; ++i) {
        if (selected[i])
        {
            cl_uint numDevices = 0;
            cl_int err = clGetDeviceIDs(platformsIds[i], CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
            if (err != CL_SUCCESS || numDevices == 0)
            {
                cerr << "No devices found for platform " << i << " or error: " << err << '\n';
                continue;
            }
    
            vector<cl_device_id> allDevices(numDevices);
            err = clGetDeviceIDs(platformsIds[i], CL_DEVICE_TYPE_ALL, numDevices, allDevices.data(), nullptr);
            if (err != CL_SUCCESS)
            {
                cerr << "Failed to get devices for platform " << i << ". Error: " << err << '\n';
                continue;
            }
            
            bool deviceSelected[numDevices];
            for(int i = 0; i < numDevices; ++i)
                deviceSelected[i] = 0;
            
            int selectedCount = 0;
            int deviceChoice = 0;
    
            while (true)
            {
                cout << "\nSelect/Deselect devices for Platform #" << i << ":\n";
                for (int j = 0; j < numDevices; ++j) {
                    size_t nameSize = 0;
                    clGetDeviceInfo(allDevices[j], CL_DEVICE_NAME, 0, nullptr, &nameSize);
                    vector<char> nameBuffer(nameSize);
                    clGetDeviceInfo(allDevices[j], CL_DEVICE_NAME, nameSize, nameBuffer.data(), nullptr);
    
                    cout << "\t[" << (deviceSelected[j] ? '*' : ' ') << "] Device #" << j << ": " << nameBuffer.data() << '\n';
                }
    
                cout << "\tOr enter " << numDevices << " to finish selection.\n";
                cout << "Enter number: ";
                cin >> deviceChoice;
    
                if (deviceChoice == numDevices)
                {
                    if (selectedCount == 0)
                        cout << "No devices selected!\n";
                    else
                        break;
                }
                else if (deviceChoice >= 0 && deviceChoice < numDevices)
                {
                    deviceSelected[deviceChoice] = !deviceSelected[deviceChoice];
                    selectedCount += deviceSelected[deviceChoice] ? 1 : -1;
                }
                else
                    cout << "Invalid choice.\n";
            }
    
            vector<Device> selectedDevices;
            for (int j = 0; j < numDevices; ++j) {
                if (deviceSelected[j])
                {
                    size_t nameSize = 0;
                    clGetDeviceInfo(allDevices[j], CL_DEVICE_NAME, 0, nullptr, &nameSize);
                    vector<char> nameBuffer(nameSize);
                    clGetDeviceInfo(allDevices[j], CL_DEVICE_NAME, nameSize, nameBuffer.data(), nullptr);

                    cl_device_type type_id;
                    clGetDeviceInfo(allDevices[j], CL_DEVICE_TYPE, sizeof(type_id), &type_id, nullptr);

                    string type;
                    if (type_id & CL_DEVICE_TYPE_CPU) type += "CPU";
                    if (type_id & CL_DEVICE_TYPE_GPU) type += "GPU";
                    if (type_id & CL_DEVICE_TYPE_ACCELERATOR) type += "Accelerator";
                    if (type_id & CL_DEVICE_TYPE_DEFAULT) type += "Default";

                    Device dev(allDevices[j], type, string(nameBuffer.data()));
                    selectedDevices.push_back(dev);
                }
            }
            size_t nameSize = 0;
            clGetPlatformInfo(platformsIds[i], CL_PLATFORM_NAME, 0, nullptr, &nameSize);
            vector<char> nameBuffer(nameSize);
            clGetPlatformInfo(platformsIds[i], CL_PLATFORM_NAME, nameSize, nameBuffer.data(), nullptr);
            string platformName(nameBuffer.data());

            platforms.push_back(Platform(platformsIds[i], platformName, selectedDevices));
        }
    }
}

void WorkingGroup::automaticallySelectPlatforms()
{
    platforms.clear();

    cl_uint numPlatforms = 0;
    cl_int err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    if (err != CL_SUCCESS || numPlatforms == 0)
    {
        cerr << "No OpenCL platforms found. Error: " << err << '\n';
        return;
    }

    vector<cl_platform_id> platformIds(numPlatforms);
    clGetPlatformIDs(numPlatforms, platformIds.data(), nullptr);

    for (int i = 0; i < numPlatforms; ++i)
    {
        cl_platform_id platformId = platformIds[i];

        size_t nameSize = 0;
        clGetPlatformInfo(platformId, CL_PLATFORM_NAME, 0, nullptr, &nameSize);
        vector<char> nameBuffer(nameSize);
        clGetPlatformInfo(platformId, CL_PLATFORM_NAME, nameSize, nameBuffer.data(), nullptr);
        string platformName(nameBuffer.data());

        cl_uint numDevices = 0;
        err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices);
        if (err != CL_SUCCESS || numDevices == 0)
        {
            cerr << "No devices found on platform: " << platformName << '\n';
            continue;
        }

        vector<cl_device_id> deviceIds(numDevices);
        clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, numDevices, deviceIds.data(), nullptr);

        vector<Device> devices;
        for (int j = 0; j < numDevices; ++j) {
            cl_device_id deviceId = deviceIds[j];

            size_t nameSize = 0;
            clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, nullptr, &nameSize);
            vector<char> nameBuffer(nameSize);
            clGetDeviceInfo(deviceId, CL_DEVICE_NAME, nameSize, nameBuffer.data(), nullptr);
            string deviceName(nameBuffer.data());

            cl_device_type typeId;
            clGetDeviceInfo(deviceId, CL_DEVICE_TYPE, sizeof(typeId), &typeId, nullptr);
            string type;
            if (typeId & CL_DEVICE_TYPE_CPU) type += "CPU";
            if (typeId & CL_DEVICE_TYPE_GPU) type += "GPU";
            if (typeId & CL_DEVICE_TYPE_ACCELERATOR) type += "Accelerator";
            if (typeId & CL_DEVICE_TYPE_DEFAULT) type += "Default";

            devices.push_back(Device(deviceId, type, deviceName));
        }

        platforms.push_back(Platform(platformId, platformName, devices));
    }

    if (platforms.empty())
        cerr << "No valid platforms with devices were found.\n";
    else
        cout << "Successfully selected " << platforms.size() << " platforms.\n";
}

void WorkingGroup::showSelectedPlatformsDevices() const
{
    cout << '\n';
    for (int i = 0; i < platforms.size(); ++i) {
        platforms[i].showPlatformInfo();
        platforms[i].showSelectedDevices();
        cout << '\n';
    }
}

void WorkingGroup::createContextAndQueues()
{
    for(int i = 0; i < platforms.size(); ++i)
        platforms[i].createContextAndQueues();
}

pair<int, int> WorkingGroup::selectOneDevice() const
{
    if(platforms.empty())
    {
        cerr << "No selected devices!\n";
        return make_pair(-1, -1);
    }

    vector<pair<int, int>> platformDeviceMap;

    cout << "\nAvalabile Devices:\n";

    int count = 0;
    for(int i = 0; i < platforms.size(); ++i)
    {
        const Platform &pd = platforms[i];
        const vector<Device> &devices = pd.getDevices();
        
        pd.showPlatformInfo();
        for(int j = 0; j < devices.size(); ++j)
        {
            cout << "\t[" << count << "] ";
            devices[j].showDeviceInfo();

            platformDeviceMap.push_back(make_pair(i, j));
            ++count;
        }
    }

    int selected = -1;
    cout << "\nSelect the device number to run on: ";
    while (true)
    {
        cin >> selected;

        if (selected < 0 || selected >= platformDeviceMap.size())
            cout << "Invalid selection. Try again: ";
        else
            break;
    }

    return platformDeviceMap[selected];
}

void WorkingGroup::runOnOneDevice(KernelExecutor &executor)
{
    pair<int, int> platformDeviceIndex;

    if(platforms.size() == 0)
    {
        cout << "No devices were selected!\n";
        return;
    }
    else if (platforms.size() == 1 && platforms[0].getDevices().size() == 1)
    {
        platformDeviceIndex = pair<int, int> (0, 0);
    }
    else
    {
        platformDeviceIndex = selectOneDevice();

        if(platformDeviceIndex.first == -1 || platformDeviceIndex.second == -1)
        {
            cerr << "Platform/Device wasn't selected properly!\n";
            return;
        }
    }
    
    const Platform &p = platforms[platformDeviceIndex.first];
    const Device &d = p.getDevices()[platformDeviceIndex.second];

    cout << "Selected Platform:\n";
    p.showPlatformInfo();
    cout << "Selected Device:\n";
    d.showDeviceInfo();

    Program& openclProgram = executor.getProgram();
    openclProgram.setContext(p.getContext());

    vector<Device*> devs = { &const_cast<Device&>(d) };
    openclProgram.setDevices(devs);

    openclProgram.compileProgram();

    executor.allocateBuffers();
    executor.execute();
}