#pragma once

#include "Arduino.h"

#include "opengloves/opengloves.hpp"

namespace SenseShift::OpenGloves {
    class Plotter {
      public:
        Plotter(Print* output, char* prefix) : output_(output), prefix_(prefix){};

        void plot(const og::InputPeripheralData data)
        {
            this->output_->printf("%sThumbCurl: ", this->prefix_);
            this->output_->printf("%4.0f", data.curl.thumb.curl_total * 4095);
            this->output_->printf(", %sIndexCurl: ", this->prefix_);
            this->output_->printf("%4.0f", data.curl.index.curl_total * 4095);
            this->output_->printf(", %sMiddleCurl: ", this->prefix_);
            this->output_->printf("%4.0f", data.curl.middle.curl_total * 4095);
            this->output_->printf(", %sRingCurl: ", this->prefix_);
            this->output_->printf("%4.0f", data.curl.ring.curl_total * 4095);
            this->output_->printf(", %sPinkyCurl: ", this->prefix_);
            this->output_->printf("%4.0f", data.curl.pinky.curl_total * 4095);

            this->output_->printf(", %sJoyX: ", this->prefix_);
            this->output_->printf("%4.0f", data.joystick.x * 4095);
            this->output_->printf(", %sJoyY: ", this->prefix_);
            this->output_->printf("%4.0f", data.joystick.y * 4095);

            this->output_->println();
        }

      private:
        Print* const output_;
        char* const prefix_;
    };
} // namespace SenseShift::OpenGloves