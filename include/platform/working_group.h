#include "common.h"

using namespace std;

enum class SelectionMode {
    Manual,
    Automatic
};

class WorkingGroup {
    private:
        vector<Platform> platforms;

        void manuallySelectPlatforms();
        void automaticallySelectPlatforms();
        void createContextAndQueues();

        pair<int, int> selectOneDevice() const;

    public:
        WorkingGroup(SelectionMode selectionMode);
        
        void showSelectedPlatformsDevices() const;

        vector<Platform> getPlatforms() { return platforms; }

        void runOnOneDevice(KernelExecutor &executor);
};