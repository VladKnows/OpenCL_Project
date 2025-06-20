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

        void runOnOneDevice();
};