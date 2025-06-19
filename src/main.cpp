#include "common.h"

using namespace std;

int main()
{
    WorkingGroup workingGroup(SelectionMode::Automatic);
    workingGroup.showSelectedPlatformsDevices();

    workingGroup.runOnOneDevice();

    return 0;
}
