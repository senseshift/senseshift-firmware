#include "openhaptics.h"

OpenHaptics::OpenHaptics()
{
    this->output = new Output();
}

void OpenHaptics::registerComponent(Component* component)
{
    if (component == nullptr) {
        return;
    }

    for (auto *c : this->components) {
        if (c == component) {
            return;
        }
    }

    this->components.push_back(component);
}

void OpenHaptics::addOutputComponent(outputPath_t path, OutputComponent* c)
{
    this->registerComponent(c);
    this->getOutput()->addComponent(path, c);
}

void OpenHaptics::setup()
{
    for (auto &component : this->components) {
        component->setup();
    }
}

void OpenHaptics::loop()
{
    for (auto *c : this->components) {
        c->loop();
    }
}

OpenHaptics App;
