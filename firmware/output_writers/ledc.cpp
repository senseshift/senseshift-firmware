#include "output_writers/ledc.h"

void LEDCOutputWriter::writeOutput(outputIntensity_t intensity)
{
    ledcWrite(chan, (uint16_t)map(intensity, 0, UINT16_MAX, 0, 4096));
}
