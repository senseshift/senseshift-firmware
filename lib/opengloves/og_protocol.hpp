#pragma once

#include <cstdint>
#include <functional>
#include <vector>

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

        constexpr Type getType() const { return this->type; }

      protected:
        Type type;
    };

    class IStringEncoded : public IEncodedInput {
      public:
        IStringEncoded(Type type) : IEncodedInput(type){};

        virtual size_t getEncodedLength() const = 0;
        virtual size_t encodeString(char* buffer) const = 0;
    };

    class IStringEncodedMemoizedSensor : public IStringEncoded {
      public:
        IStringEncodedMemoizedSensor(Type type) : IStringEncoded(type){};

        virtual void init() = 0;
        virtual void updateValue() = 0;
    };

    class ICommunication {
      public:
        virtual void setup() = 0;
        virtual void send(std::vector<IStringEncodedMemoizedSensor*>& sensors) = 0;
        virtual bool hasData() = 0;
        virtual size_t readCommand(char* buffer, size_t length) = 0;
    };

    typedef uint16_t CommandIndex_t;
    typedef enum Command : CommandIndex_t {
        ThumbCurl,
        ThumbSplay,

        IndexCurl,
        IndexSplay,

        MiddleCurl,
        MiddleSplay,

        RingCurl,
        RingSplay,

        PinkyCurl,
        PinkySplay,
    } Command;

    typedef std::function<void(Command, uint16_t)> CommandCallback;
} // namespace OpenGloves
