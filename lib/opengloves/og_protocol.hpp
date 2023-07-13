#pragma once

namespace OpenGloves {
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

        IEncodedInput(Type type) : type(type){};

        constexpr Type getType() const
        {
            return this->type;
        }

      private:
        Type type;
    };

    class IStringEncodedMemoizedSensor : public IEncodedInput {
      public:
        IStringEncodedMemoizedSensor(Type type) : IEncodedInput(type){};

        virtual void setup() = 0;
        virtual void updateValue() = 0;

        virtual size_t getEncodedLength() const = 0;
        virtual size_t encodeString(char* buffer) const = 0;
    };

    class ICommunication {
      public:
        virtual void setup() = 0;
        virtual void send(std::vector<IStringEncodedMemoizedSensor*>& sensors) = 0;
    };
} // namespace OpenGloves
