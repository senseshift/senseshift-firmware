#include "output_writers/pca9685.hpp"
#include <utility.hpp>

void OH::PCA9685OutputWriter::writeOutput(oh_output_intensity_t intensity) {
  this->driver->setPin(this->num, simpleMap<uint16_t>(intensity, OH_OUTPUT_INTENSITY_MAX, 4095));
}
