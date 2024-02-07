#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

#ifdef ARDUINO
#include <WString.h>
#endif // ARDUINO

#ifdef OG_USE_FROZEN
#include <frozen/map.h>
#include <frozen/string.h>
#endif

#ifndef OG_BUFFER_SIZE
#define OG_BUFFER_SIZE 256
#endif // OG_BUFFER_SIZE

namespace og {
    using HandIndex = std::uint8_t;
    enum Hand : HandIndex {
        Hand_Left,
        Hand_Right,
    };

    using DeviceTypeIndex = std::uint8_t;
    enum DeviceType : DeviceTypeIndex {
        DeviceType_LucidGloves,
    };

    union InputFingerCurlData {
        std::array<float, 4> curl; // NOLINT(*-magic-numbers): I'm sure our finger aren't changing anytime soon
        struct {
            float curl_total;
            float curl_joint1;
            float curl_joint2;
            float curl_joint3;
        };
    };

    template<typename Tp>
    union InputFingerData {
        std::array<Tp, 5> fingers; // NOLINT(*-magic-numbers): We aren't going to grow any new fingers soon tbh
        struct {
            Tp thumb;
            Tp index;
            Tp middle;
            Tp ring;

            union {
                Tp pinky;
                Tp little;
            };
        };
    };

    struct InputJoystickData {
        float x, y;
        bool press;
    };

    struct InputButtonData {
        bool press;
        // bool touch;
    };

    struct InputAnalogButtonData : InputButtonData {
        float value;
    };

    /// Input data structure.
    ///
    /// I know, it is not the prettiest one, but we need this type of punning to efficiently encode/decode the data
    struct InputPeripheralData {
        InputFingerData<InputFingerCurlData> curl;
        InputFingerData<float> splay;

        InputJoystickData joystick;

        union {
            std::array<InputButtonData, 4> buttons;
            struct {
                InputButtonData button_a;
                InputButtonData button_b;
                InputButtonData button_menu;
                InputButtonData button_calibrate;
                InputButtonData pinch;
            };
        };

        union {
            std::array<InputAnalogButtonData, 2> analog_buttons;
            struct {
                InputAnalogButtonData trigger;
                InputAnalogButtonData grab;
            };
        };
    };

    struct InputInfoData {
        Hand hand;
        DeviceType device_type;

        unsigned int firmware_version;
    };

    using Input = std::variant<InputInfoData, InputPeripheralData>;

    class Output {};

    class IEncoder {
      public:
        [[nodiscard]] virtual auto encode_input(const Input& input, char* buffer, size_t length) const -> size_t = 0;

        [[nodiscard]] auto encode_input(const Input& input) const -> std::string
        {
            std::string buffer;
            buffer.resize(OG_BUFFER_SIZE);

            const auto length = this->encode_input(input, buffer.data(), buffer.length());
            buffer.resize(length);

            return buffer;
        }

        [[nodiscard]] virtual auto parse_output(const char* data, size_t length) const -> Output = 0;

        [[nodiscard]] auto parse_output(const std::vector<char>& data) const -> Output
        {
            return this->parse_output(data.data(), data.size());
        }

        [[nodiscard]] auto parse_output(const std::string& data) const -> Output
        {
            return this->parse_output(data.data(), data.length());
        }

#ifdef ARDUINO
        [[nodiscard]] auto parse_output(const String& data) const -> Output
        {
            return this->parse_output(data.c_str(), data.length());
        }
#endif // ARDUINO
    };

    class AlphaEncoder : public IEncoder {
      public:
        using CommandIndex = std::uint8_t;
        enum Command : CommandIndex {
            FingerThumb_CurlTotal,
            FingerThumb_Splay,
            FingerThumb_CurlJoint0,
            FingerThumb_CurlJoint1,
            FingerThumb_CurlJoint2,
            FingerThumb_CurlJoint3 [[maybe_unused]],

            FingerIndex_CurlTotal,
            FingerIndex_Splay,
            FingerIndex_CurlJoint0,
            FingerIndex_CurlJoint1,
            FingerIndex_CurlJoint2,
            FingerIndex_CurlJoint3,

            FingerMiddle_CurlTotal,
            FingerMiddle_Splay,
            FingerMiddle_CurlJoint0,
            FingerMiddle_CurlJoint1,
            FingerMiddle_CurlJoint2,
            FingerMiddle_CurlJoint3,

            FingerRing_CurlTotal,
            FingerRing_Splay,
            FingerRing_CurlJoint0,
            FingerRing_CurlJoint1,
            FingerRing_CurlJoint2,
            FingerRing_CurlJoint3,

            FingerPinky_CurlTotal,
            FingerPinky_Splay,
            FingerPinky_CurlJoint0,
            FingerPinky_CurlJoint1,
            FingerPinky_CurlJoint2,
            FingerPinky_CurlJoint3,

            MainJoystick_X,
            MainJoystick_Y,
            MainJoystick_Click,

            ButtonA_Click,
            ButtonB_Click,
            ButtonMenu_Click,
            ButtonCalibrate_Click,

            GestureTrigger_Value,
            GestureTrigger_Click,

            GestureGrab_Click,

            GesturePinch_Click,

            Info,
            Info_StartStreaming,
            Info_StopStreaming,
            Info_FirmwareVersion,
            Info_DeviceType,
            Info_Hand,

            Haptics_Frequency,
            Haptics_Duration,
            Haptics_Amplitude,
        };

        inline static constexpr const char* INFO_FIRMWARE_VERSION_KEY = "(ZV)";
        inline static constexpr const char* INFO_DEVICE_TYPE_KEY = "(ZG)";
        inline static constexpr const char* INFO_HAND_KEY = "(ZH)";

        inline static constexpr const uint16_t MAX_ANALOG_VALUE = 4096;

#ifdef OG_USE_FROZEN
        inline static constexpr const auto ALPHA_KEYS_TO_COMMAND = frozen::make_map<frozen::string, Command>
#else
        inline static const auto ALPHA_KEYS_TO_COMMAND = std::map<std::string, Command>
#endif

          ({
            { "A", Command::FingerThumb_CurlTotal },  // Thumb finger force feedback
            { "B", Command::FingerIndex_CurlTotal },  // Index finger force feedback
            { "C", Command::FingerMiddle_CurlTotal }, // Middle finger force feedback
            { "D", Command::FingerRing_CurlTotal },   // Ring finger force feedback
            { "E", Command::FingerPinky_CurlTotal },  // Pinky finger force feedback

            { "F", Command::Haptics_Frequency }, //
            { "G", Command::Haptics_Duration },  //
            { "H", Command::Haptics_Amplitude }, //

            { "Z", Command::Info },
          });

#ifdef OG_USE_FROZEN
        inline static constexpr const auto COMMAND_TO_ALPHA_KEY = frozen::make_map<Command, frozen::string>
#else
        inline static const auto COMMAND_TO_ALPHA_KEY = std::map<Command, std::string>
#endif
          ({
            { Command::FingerThumb_CurlTotal, "A" },      // Whole thumb curl
            { Command::FingerThumb_Splay, "(AB)" },       // Whole thumb splay
            { Command::FingerThumb_CurlJoint0, "(AAA)" }, //
            { Command::FingerThumb_CurlJoint1, "(AAB)" }, //
            { Command::FingerThumb_CurlJoint2, "(AAC)" }, //
            // { Command::FingerThumb_CurlJoint3, "(AAD)" }, // not used

            { Command::FingerIndex_CurlTotal, "B" },      //
            { Command::FingerIndex_Splay, "(BB)" },       //
            { Command::FingerIndex_CurlJoint0, "(BAA)" }, //
            { Command::FingerIndex_CurlJoint1, "(BAB)" }, //
            { Command::FingerIndex_CurlJoint2, "(BAC)" }, //
            { Command::FingerIndex_CurlJoint3, "(BAD)" }, //

            { Command::FingerMiddle_CurlTotal, "C" },      //
            { Command::FingerMiddle_Splay, "(CB)" },       //
            { Command::FingerMiddle_CurlJoint0, "(CAA)" }, //
            { Command::FingerMiddle_CurlJoint1, "(CAB)" }, //
            { Command::FingerMiddle_CurlJoint2, "(CAC)" }, //
            { Command::FingerMiddle_CurlJoint3, "(CAD)" }, //

            { Command::FingerRing_CurlTotal, "D" },      //
            { Command::FingerRing_Splay, "(DB)" },       //
            { Command::FingerRing_CurlJoint0, "(DAA)" }, //
            { Command::FingerRing_CurlJoint1, "(DAB)" }, //
            { Command::FingerRing_CurlJoint2, "(DAC)" }, //
            { Command::FingerRing_CurlJoint3, "(DAD)" }, //

            { Command::FingerPinky_CurlTotal, "E" },      //
            { Command::FingerPinky_Splay, "(EB)" },       //
            { Command::FingerPinky_CurlJoint0, "(EAA)" }, //
            { Command::FingerPinky_CurlJoint1, "(EAB)" }, //
            { Command::FingerPinky_CurlJoint2, "(EAC)" }, //
            { Command::FingerPinky_CurlJoint3, "(EAD)" }, //

            { Command::MainJoystick_X, "F" },     //
            { Command::MainJoystick_Y, "G" },     //
            { Command::MainJoystick_Click, "H" }, //

            { Command::GestureTrigger_Click, "I" }, // Binary trigger button/gesture

            { Command::ButtonA_Click, "J" }, //
            { Command::ButtonB_Click, "K" }, //

            { Command::GestureGrab_Click, "L" },  //
            { Command::GesturePinch_Click, "M" }, //

            { Command::ButtonMenu_Click, "N" },      //
            { Command::ButtonCalibrate_Click, "O" }, //

            { Command::GestureTrigger_Value, "P" }, // Analog trigger button/gesture

            { Command::Info, "Z" },
            { Command::Info_FirmwareVersion, INFO_FIRMWARE_VERSION_KEY },
            { Command::Info_DeviceType, INFO_DEVICE_TYPE_KEY },
            { Command::Info_Hand, INFO_HAND_KEY },
          });

        /// Alpha keys for fingers.
        /// <b>MUST</b> be in the same order as the `InputFingerData` struct.
        inline static constexpr const std::array<unsigned char, 5> FINGER_ALPHA_KEY = { {
          'A', // Thumb
          'B', // Index
          'C', // Middle
          'D', // Ring
          'E', // Pinky
        } };

        /// Alpha keys for finger curl joints. Top level is the finger, second level is the joint.
        /// The top level <b>MUST</b> be in the same order as the `InputFingerData` struct.
        /// Second level array <b>MUST</b> be in the same order as the `InputFingerCurlData` struct.
        inline static const std::array<std::array<std::string, 4>, 5> FINGER_CURL_JOINT_ALPHA_KEY = { {
          { "A", "(AAB)", "(AAC)", "(AAD)" }, // Thumb (total, joint1, joint2, joint3)
          { "B", "(BAB)", "(BAC)", "(BAD)" }, // Index (total, joint1, joint2, joint3)
          { "C", "(CAB)", "(CAC)", "(CAD)" }, // Middle (total, joint1, joint2, joint3)
          { "D", "(DAB)", "(DAC)", "(DAD)" }, // Ring (total, joint1, joint2, joint3)
          { "E", "(EAB)", "(EAC)", "(EAD)" }, // Pinky (total, joint1, joint2, joint3)
        } };

        /// Alpha keys for buttons.
        /// <b>MUST</b> be in the same order as the `InputPeripheralData` struct.
        inline static constexpr const std::array<unsigned char, 5> BUTTON_ALPHA_KEY = { {
          'J', // Button A
          'K', // Button B
          'N', // Button Menu
          'O', // Button Calibrate
          'M', // Button Pinch
        } };

        /// Alpha keys for analog buttons.
        /// <b>MUST</b> be in the same order as the `InputPeripheralData` struct.
        inline static constexpr const std::array<unsigned char, 2> ANALOG_BUTTON_ALPHA_KEY = { {
          'P', // Trigger
          'I', // Grab
        } };

        auto encode_input(const Input& input, char* buffer, size_t length) const -> size_t override;

        auto parse_output(const char* data, size_t length) const -> Output override;
    };
} // namespace og