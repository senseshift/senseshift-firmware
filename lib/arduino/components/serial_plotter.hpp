#pragma once

#include <haptic_body.hpp>
#include <task.hpp>

#include <HardwareSerial.h>

#ifndef SERIAL_PLOTTER_BAUD
#define SERIAL_PLOTTER_BAUD 115200
#endif // SERIAL_PLOTTER_BAUD

namespace OH {
    /**
     * Component, that prints the current state of the output to the serial port in Arduino's Serial Plotter format
     *
     * @tparam _Tp the type of the serial port
     */
    template<class _Tp>
    class SerialPlotter_OutputStates : public Task<SerialPlotter_OutputStates<_Tp>> {
        friend class Task<SerialPlotter_OutputStates<_Tp>>;

      private:
        _Tp* serial;
        HapticBody* output;
        uint32_t sampleRate;

        void setup(void){};
        void run(void);

      public:
        SerialPlotter_OutputStates(
          _Tp& serial,
          HapticBody* output,
          uint32_t sampleRate,
          TaskConfig taskConfig = { "Serial Plotter", 2048, 1, tskNO_AFFINITY }
        ) :
          Task<SerialPlotter_OutputStates<_Tp>>(taskConfig), serial(&serial), output(output), sampleRate(sampleRate){};
        SerialPlotter_OutputStates(_Tp& serial, HapticBody* output) : SerialPlotter_OutputStates(serial, output, 100){};

        void begin() override
        {
            this->setup();
            OH::Task<SerialPlotter_OutputStates<_Tp>>::begin();
        };
    };

    /**
     * Specialized setup for HardwareSerial
     */
    template<>
    inline void SerialPlotter_OutputStates<HardwareSerial>::setup()
    {
        this->serial->begin(SERIAL_PLOTTER_BAUD);
    }

    template class SerialPlotter_OutputStates<HardwareSerial>;
} // namespace OH
