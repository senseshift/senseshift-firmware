#ifndef OUTPUT_H
#define OUTPUT_H

class Output
{
    virtual void setup(void) = 0;
    virtual void writeMotors(uint8_t[]) = 0;
    virtual void writeMotor(uint8_t, float) = 0;
};

#endif /* OUTPUT_H */
