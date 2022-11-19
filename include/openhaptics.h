#pragma once

#include <vector>

#include "config/battery.h"
#include "component.h"
#include "connection.h"
#include "output.h"

#if defined(BATERY_ENABLED) && BATERY_ENABLED == 1
#include "battery/abstract_battery.h"
#endif

class OpenHaptics final
{
    private:
        std::vector<Component*> components {};
        Connection* connection;
        Output* output;

        #if defined(BATERY_ENABLED) && BATERY_ENABLED == 1
        AbstractBattery* battery;
        #endif

    public:
        OpenHaptics();
        std::vector<Component*> getComponents() { return this->components; };
        void registerComponent(Component*);

        Output* getOutput() { return this->output; };
        void addOutputComponent(outputPath_t, OutputComponent*);

        void setConnection(Connection*);
        Connection* getConnection() { return this->connection; };

        #if defined(BATERY_ENABLED) && BATERY_ENABLED == 1
        AbstractBattery* getBattery() { return this->battery; };
        #endif

        void setup(void);
        void loop(void);
};

extern OpenHaptics App;
