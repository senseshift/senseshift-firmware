#include "haptic_body.hpp"

#include <logging.hpp>

void OH::HapticBody::addComponent(const oh_output_path_t path, OH::HapticPlane* c)
{
    this->components[path] = c;
}

std::map<oh_output_path_t, OH::HapticPlane*>* OH::HapticBody::getComponents()
{
    return &this->components;
}

void OH::HapticBody::writeOutput(const oh_output_path_t path, const oh_output_data_t& data)
{
    if (this->getComponents()->count(path) == 0) {
        // if no requested component exists, skip
        log_w("No component found for path %d", path);
        return;
    }

    auto componentSearch = this->getComponents()->find(path);
    (*componentSearch).second->writeOutput(data);
}
