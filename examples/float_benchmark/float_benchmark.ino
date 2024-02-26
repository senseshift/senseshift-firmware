#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <inttypes.h>

#define TIMES_TO_LOOP 1000000

/** by Vlad Kaipetsky
portable assuming FP24 set to nearest rounding mode
efficient on x86 platform
*/
inline int toInt(float fval)
{
    static const float Snapper = 3 << 22;

    union UFloatInt {
        int i;
        float f;
    };

    assert(fabs(fval) <= 0x003fffff); // only 23 bit values handled
    UFloatInt& fi = *(UFloatInt*) &fval;
    fi.f += Snapper;
    return ((fi.i) & 0x007fffff) - 0x00400000;
}

inline auto ifloor_toInt(float x) -> int
{
    const auto casted = toInt(x);
    return casted - (x < casted);
}

inline int i_cast(float value)
{
    return static_cast<int>(value);
}

inline auto ifloor_cast(float x) -> int
{
    const auto casted = i_cast(x);
    return casted - (x < casted);
}

inline int ifloor_std_cast(float x)
{
    return static_cast<int>(std::floor(x));
}

volatile int xi, yi, resulti;
volatile float xf, yf, resultf;

uint32_t seed32()
{
    return random(0, 0xffffffff);
}

float seedfloat()
{
    // float x, y;

    // x = seed32();
    // y = seed32();

    // return x / y;
    return 4094.8492342342932034F;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Preparing system...");

    // Turn WiFi off for consistency
    WiFi.mode(WIFI_OFF);

    delay(1000);
    Serial.println("Starting benchmark");

    uint32_t i;
    uint64_t micros_start, micros_end;

    xf = seedfloat();
    yf = seedfloat();

    micros_start = micros();
    for (i = 0; i < TIMES_TO_LOOP; i++)
        resulti = i_cast(xf);
    micros_end = micros();
    Serial.printf("i_cast(%f) => %i\t\tmicros: ", xf, resulti);
    Serial.println(micros_end - micros_start);

    micros_start = micros();
    for (i = 0; i < TIMES_TO_LOOP; i++)
        resulti = ifloor_cast(xf);
    micros_end = micros();
    Serial.printf("ifloor_cast(%f) => %i\tmicros: ", xf, resulti);
    Serial.println(micros_end - micros_start);

    micros_start = micros();
    for (i = 0; i < TIMES_TO_LOOP; i++)
        resulti = ifloor_std_cast(xf);
    micros_end = micros();
    Serial.printf("ifloor_std_cast(%f) => %i\tmicros: ", xf, resulti);
    Serial.println(micros_end - micros_start);

    micros_start = micros();
    for (i = 0; i < TIMES_TO_LOOP; i++)
        resulti = toInt(xf);
    micros_end = micros();
    Serial.printf("toInt(%f) => %i\t\tmicros: ", xf, resulti);
    Serial.println(micros_end - micros_start);

    micros_start = micros();
    for (i = 0; i < TIMES_TO_LOOP; i++)
        resulti = ifloor_toInt(xf);
    micros_end = micros();
    Serial.printf("ifloor_toInt(%f) => %i\tmicros: ", xf, resulti);
    Serial.println(micros_end - micros_start);
}

void loop()
{
    // put your main code here, to run repeatedly:
}
