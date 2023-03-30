#include "output_writers/pca9685.hpp"

void OH::PCA9685OutputWriter::writeOutput(oh_output_intensity_t intensity) {
  this->driver->setPin(this->num, (uint16_t)map(intensity, 0, OH_OUTPUT_INTENSITY_MAX, 0, 4095));
}
