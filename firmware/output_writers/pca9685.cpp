#include "output_writers/pca9685.h"

void PCA9685OutputWriter::writeOutput(outputIntensity_t intensity)
{
    this->driver->setPin(this->num, (uint16_t)map(intensity, 0, UINT16_MAX, 0, 4095));
}
