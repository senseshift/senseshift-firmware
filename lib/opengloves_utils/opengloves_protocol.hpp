#pragma once

namespace OpenGloves
{
  struct EncodedInput {
    public:
      enum Type : char {
        THUMB = 'A',
        INDEX = 'B',
        MIDDLE = 'C',
        RING = 'D',
        PINKY = 'E',
        JOY_X = 'F',
        JOY_Y = 'G',
        JOY_BTN = 'H',
        TRIGGER = 'I',
        A_BTN = 'J',
        B_BTN = 'K',
        GRAB = 'L',
        PINCH = 'M',
        MENU = 'N',
        CALIBRATE = 'O'
      };

      EncodedInput(Type type) : type(type) { };

      constexpr Type getType() const {
        return this->type;
      }

      virtual size_t getEncodedSize() const = 0;
      virtual int encode(char* buffer) const = 0;

    private:
      Type type;
  };
} // namespace OpenGloves
