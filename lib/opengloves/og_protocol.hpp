#pragma once

namespace OpenGloves
{
  struct IEncodedInput {
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

      IEncodedInput(Type type) : type(type) { };

      constexpr Type getType() const {
        return this->type;
      }

      virtual size_t getEncodedSize() const = 0;
      virtual int encode(char* buffer) const = 0;

    private:
      Type type;
  };

  class IEncodedSensor : public IEncodedInput {
    public:
      IEncodedSensor(Type type) : IEncodedInput(type) { };

      virtual void setup() = 0;
      virtual void updateValue() = 0;
  };

  class ICommunication {
    public:
      virtual void setup() = 0;
      virtual void send(std::vector<IEncodedSensor*> sensors) = 0;
  };
} // namespace OpenGloves
