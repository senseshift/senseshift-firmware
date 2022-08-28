#include "output_writers/direct.h"

void DirectOutputWriter::writeOutput(outputIntensity_t intensity)
{
    // if (intensity) {
    //     printf("PWM %d %d\n", this->chan, intensity);
    // }
    ledcWrite(chan, (uint16_t)map(intensity, 0, UINT16_MAX, 0, 4096));
}
