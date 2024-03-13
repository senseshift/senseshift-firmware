#pragma once

#include <senseshift/body/hands/input/gesture.hpp>
#include <senseshift/body/hands/input/total_curl.hpp>
#include <senseshift/input/calibration.hpp>
#include <senseshift/input/filter.hpp>
#include <senseshift/input/sensor/sensor.hpp>
#include <senseshift/opengloves/constants.hpp>
#include <senseshift/opengloves/opengloves.hpp>

#ifdef ARDUINO
#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/input/sensor/digital.hpp>
#include <senseshift/arduino/input/sensor/multiplexer.hpp>
#include <senseshift/arduino/output/servo.hpp>
#include <senseshift/opengloves/transport/stream.hpp>
#endif

#pragma region Communication

#ifndef OPENGLOVES_COMMUNICATION
#define OPENGLOVES_COMMUNICATION OPENGLOVES_COMM_SERIAL
#endif

#ifndef SERIAL_PORT
#define SERIAL_PORT Serial
#endif

#ifndef SERIAL_BAUDRATE
#define SERIAL_BAUDRATE 115200
#endif

#ifndef BTSERIAL_PREFIX
#define BTSERIAL_PREFIX "SenseShift_OG"
#endif

#pragma endregion

#pragma region Calibration

#ifndef CALIBRATION_CURL
#define CALIBRATION_CURL new ::SenseShift::Input::Calibration::MinMaxCalibrator<float>()
#endif
#ifndef CALIBRATION_SPLAY
#define CALIBRATION_SPLAY new ::SenseShift::Input::Calibration::CenterPointDeviationCalibrator<float>(0.66F, 0.005F)
#endif

#ifndef CALIBRATION_DURATION
#define CALIBRATION_DURATION 2000 // duration in milliseconds
#endif

#pragma endregion

#pragma region Fingers

#ifdef PIN_FINGER_THUMB
#define FINGER_THUMB_ENABLED (PIN_FINGER_THUMB != -1)
#else
#define FINGER_THUMB_ENABLED false
#endif

#ifdef PIN_FINGER_INDEX
#define FINGER_INDEX_ENABLED (PIN_FINGER_INDEX != -1)
#else
#define FINGER_INDEX_ENABLED false
#endif

#ifdef PIN_FINGER_MIDDLE
#define FINGER_MIDDLE_ENABLED (PIN_FINGER_MIDDLE != -1)
#else
#define FINGER_MIDDLE_ENABLED false
#endif

#ifdef PIN_FINGER_RING
#define FINGER_RING_ENABLED (PIN_FINGER_RING != -1)
#else
#define FINGER_RING_ENABLED false
#endif

#ifdef PIN_FINGER_PINKY
#define FINGER_PINKY_ENABLED (PIN_FINGER_PINKY != -1)
#else
#define FINGER_PINKY_ENABLED false
#endif

#define DEFINE_FINGER(NAME, CURL_PIN, CURL_INVERT, CURL_CALIB)                                                        \
    auto* NAME##_sensor =                                                                                             \
      new ::SenseShift::Input::SimpleSensorDecorator(new ::SenseShift::Arduino::Input::AnalogSimpleSensor(CURL_PIN)); \
    NAME##_sensor->addFilters({ new ::SenseShift::Input::Filter::ExponentialMovingAverageFilter<float>(0.8F) });      \
    if (CURL_INVERT) {                                                                                                \
        NAME##_sensor->addFilter({ new ::SenseShift::Input::Filter::AnalogInvertFilter() });                          \
    }                                                                                                                 \
    NAME##_sensor->setCalibrator((CURL_CALIB));

#ifdef PIN_FINGER_THUMB_SPLAY
#define FINGER_THUMB_SPLAY (FINGER_THUMB_ENABLED && (PIN_FINGER_THUMB_SPLAY != -1))
#else
#define FINGER_THUMB_SPLAY false
#endif

#ifdef PIN_FINGER_INDEX_SPLAY
#define FINGER_INDEX_SPLAY (FINGER_INDEX_ENABLED && (PIN_FINGER_INDEX_SPLAY != -1))
#else
#define FINGER_INDEX_SPLAY false
#endif

#ifdef PIN_FINGER_MIDDLE_SPLAY
#define FINGER_MIDDLE_SPLAY (FINGER_MIDDLE_ENABLED && (PIN_FINGER_MIDDLE_SPLAY != -1))
#else
#define FINGER_MIDDLE_SPLAY false
#endif

#ifdef PIN_FINGER_RING_SPLAY
#define FINGER_RING_SPLAY (FINGER_RING_ENABLED && (PIN_FINGER_RING_SPLAY != -1))
#else
#define FINGER_RING_SPLAY false
#endif

#ifdef PIN_FINGER_PINKY_SPLAY
#define FINGER_PINKY_SPLAY (FINGER_PINKY_ENABLED && (PIN_FINGER_PINKY_SPLAY != -1))
#else
#define FINGER_PINKY_SPLAY false
#endif

#define DEFINE_FINGER_SPLAY(NAME, CURL_PIN, CURL_INVERT, CURL_CALIB, SPLAY_PIN, SPLAY_INVERT, SPLAY_CALIB) \
    DEFINE_FINGER(NAME##_curl, CURL_PIN, CURL_INVERT, CURL_CALIB);                                         \
    DEFINE_FINGER(NAME##_splay, SPLAY_PIN, SPLAY_INVERT, SPLAY_CALIB);

#pragma endregion

#pragma region Joysticks

#if defined(PIN_JOYSTICK_X) && defined(PIN_JOYSTICK_Y)
#define JOYSTICK_ENABLED (PIN_JOYSTICK_X != -1) && (PIN_JOYSTICK_Y != -1)
#else
#define JOYSTICK_ENABLED false
#endif

#define DEFINE_JOYSTICK_AXIS(NAME, PIN, INVERT, DEADZONE)                                                        \
    auto NAME##_sensor =                                                                                         \
      new ::SenseShift::Input::SimpleSensorDecorator(new ::SenseShift::Arduino::Input::AnalogSimpleSensor(PIN)); \
    NAME##_sensor->addFilters({ new ::SenseShift::Input::Filter::ExponentialMovingAverageFilter<float>(0.7F),    \
                                new ::SenseShift::Input::Filter::CenterDeadzoneFilter(DEADZONE) });              \
    if (INVERT) {                                                                                                \
        NAME##_sensor->addFilter({ new ::SenseShift::Input::Filter::AnalogInvertFilter() });                     \
    }
#pragma endregion

#pragma region Buttons

#ifdef PIN_BUTTON_A
#define BUTTON_A_ENABLED (PIN_BUTTON_A != -1)
#else
#define BUTTON_A_ENABLED false
#endif

#ifdef PIN_BUTTON_B
#define BUTTON_B_ENABLED (PIN_BUTTON_B != -1)
#else
#define BUTTON_B_ENABLED false
#endif

#ifdef PIN_BUTTON_MENU
#define BUTTON_MENU_ENABLED (PIN_BUTTON_MENU != -1)
#else
#define BUTTON_MENU_ENABLED false
#endif

#ifdef PIN_BUTTON_JOYSTICK
#define BUTTON_JOYSTICK_ENABLED (PIN_BUTTON_JOYSTICK != -1)
#else
#define BUTTON_JOYSTICK_ENABLED false
#endif

#ifdef PIN_BUTTON_CALIBRATE
#define BUTTON_CALIBRATE_ENABLED (PIN_BUTTON_CALIBRATE != -1)
#else
#define BUTTON_CALIBRATE_ENABLED false
#endif

#ifdef PIN_BUTTON_TRIGGER
#define BUTTON_TRIGGER_ENABLED (PIN_BUTTON_TRIGGER != -1)
#else
#define BUTTON_TRIGGER_ENABLED false
#endif

#ifdef PIN_BUTTON_GRAB
#define BUTTON_GRAB_ENABLED (PIN_BUTTON_GRAB != -1)
#else
#define BUTTON_GRAB_ENABLED false
#endif

#ifdef PIN_BUTTON_PINCH
#define BUTTON_PINCH_ENABLED (PIN_BUTTON_PINCH != -1)
#else
#define BUTTON_PINCH_ENABLED false
#endif

#define BUTTON_CLASS(PIN, MODE, INVERT)                                                       \
    ::SenseShift::Input::SimpleSensorDecorator(                                               \
      new ::SenseShift::Arduino::Input::DigitalSimpleSensor(PIN, MODE, (INVERT ? HIGH : LOW)) \
    );

#pragma endregion

#pragma region Gestures

#ifndef GESTURE_TRIGGER_THRESHOLD
#define GESTURE_TRIGGER_THRESHOLD (0.5F)
#endif

#ifndef GESTURE_GRAB_THRESHOLD
#define GESTURE_GRAB_THRESHOLD (0.5F)
#endif

#ifndef GESTURE_PINCH_THRESHOLD
#define GESTURE_PINCH_THRESHOLD (0.5F)
#endif

#pragma endregion

#ifdef PIN_FFB_THUMB
#define FFB_THUMB_ENABLED (PIN_FFB_THUMB != -1)
#else
#define FFB_THUMB_ENABLED false
#endif

#ifdef PIN_FFB_INDEX
#define FFB_INDEX_ENABLED (PIN_FFB_INDEX != -1)
#else
#define FFB_INDEX_ENABLED false
#endif

#ifdef PIN_FFB_MIDDLE
#define FFB_MIDDLE_ENABLED (PIN_FFB_MIDDLE != -1)
#else
#define FFB_MIDDLE_ENABLED false
#endif

#ifdef PIN_FFB_RING
#define FFB_RING_ENABLED (PIN_FFB_RING != -1)
#else
#define FFB_RING_ENABLED false
#endif

#ifdef PIN_FFB_PINKY
#define FFB_PINKY_ENABLED (PIN_FFB_PINKY != -1)
#else
#define FFB_PINKY_ENABLED false
#endif

#define FFB_ENABLED \
    (FFB_THUMB_ENABLED || FFB_INDEX_ENABLED || FFB_MIDDLE_ENABLED || FFB_RING_ENABLED || FFB_PINKY_ENABLED)

#ifndef CALIBRATION_ALWAYS_CALIBRATE
#define CALIBRATION_ALWAYS_CALIBRATE false
#endif

#ifndef UPDATE_RATE
#define UPDATE_RATE 90
#endif

namespace SenseShift::OpenGloves::AutoConfig {

    [[nodiscard]] auto createInput() -> InputSensors
    {
        InputSensors input_sensors;

#if FINGER_THUMB_SPLAY
        DEFINE_FINGER_SPLAY(
          thumb,
          PIN_FINGER_THUMB,
          FINGER_THUMB_INVERT,
          CALIBRATION_CURL,
          PIN_FINGER_THUMB_SPLAY,
          FINGER_THUMB_SPLAY_INVERT,
          CALIBRATION_SPLAY
        );
#elif FINGER_THUMB_ENABLED
        DEFINE_FINGER(thumb_curl, PIN_FINGER_THUMB, FINGER_THUMB_INVERT, CALIBRATION_CURL);
        input_sensors.curl.thumb.curl_total = thumb_curl_sensor;
#endif

#if FINGER_INDEX_SPLAY
        DEFINE_FINGER_SPLAY(
          index,
          PIN_FINGER_INDEX,
          FINGER_INDEX_INVERT,
          CALIBRATION_CURL,
          PIN_FINGER_INDEX_SPLAY,
          FINGER_INDEX_SPLAY_INVERT,
          CALIBRATION_SPLAY
        );
#elif FINGER_INDEX_ENABLED
        DEFINE_FINGER(index_curl, PIN_FINGER_INDEX, FINGER_INDEX_INVERT, CALIBRATION_CURL);
        input_sensors.curl.index.curl_total = index_curl_sensor;
#endif

#if FINGER_MIDDLE_SPLAY
        DEFINE_FINGER_SPLAY(
          middle,
          PIN_FINGER_MIDDLE,
          FINGER_MIDDLE_INVERT,
          CALIBRATION_CURL,
          PIN_FINGER_MIDDLE_SPLAY,
          FINGER_MIDDLE_SPLAY_INVERT,
          CALIBRATION_SPLAY
        );
#elif FINGER_MIDDLE_ENABLED
        DEFINE_FINGER(middle_curl, PIN_FINGER_MIDDLE, FINGER_MIDDLE_INVERT, CALIBRATION_CURL);
        input_sensors.curl.middle.curl_total = middle_curl_sensor;
#endif

#if FINGER_RING_SPLAY
        DEFINE_FINGER_SPLAY(
          ring,
          PIN_FINGER_RING,
          FINGER_RING_INVERT,
          CALIBRATION_CURL,
          PIN_FINGER_RING_SPLAY,
          FINGER_RING_SPLAY_INVERT,
          CALIBRATION_SPLAY
        );
#elif FINGER_RING_ENABLED
        DEFINE_FINGER(ring_curl, PIN_FINGER_RING, FINGER_RING_INVERT, CALIBRATION_CURL);
        input_sensors.curl.ring.curl_total = ring_curl_sensor;
#endif

#if FINGER_PINKY_SPLAY
        DEFINE_FINGER_SPLAY(
          pinky,
          PIN_FINGER_PINKY,
          FINGER_PINKY_INVERT,
          CALIBRATION_CURL,
          PIN_FINGER_PINKY_SPLAY,
          FINGER_PINKY_SPLAY_INVERT,
          CALIBRATION_SPLAY
        );
#elif FINGER_PINKY_ENABLED
        DEFINE_FINGER(pinky_curl, PIN_FINGER_PINKY, FINGER_PINKY_INVERT, CALIBRATION_CURL);
        input_sensors.curl.pinky.curl_total = pinky_curl_sensor;
#endif

#if JOYSTICK_ENABLED
        DEFINE_JOYSTICK_AXIS(joystick_x, PIN_JOYSTICK_X, JOYSTICK_X_INVERT, JOYSTICK_DEADZONE);
        DEFINE_JOYSTICK_AXIS(joystick_y, PIN_JOYSTICK_Y, JOYSTICK_Y_INVERT, JOYSTICK_DEADZONE);

        input_sensors.joystick.x = joystick_x_sensor;
        input_sensors.joystick.y = joystick_y_sensor;
#endif

#if BUTTON_A_ENABLED
        auto* button_a = new BUTTON_CLASS(PIN_BUTTON_A, INPUT_PULLUP, BUTTON_A_INVERT);
        input_sensors.button_a.press = button_a;
#endif

#if BUTTON_B_ENABLED
        auto* button_b = new BUTTON_CLASS(PIN_BUTTON_B, INPUT_PULLUP, BUTTON_B_INVERT);
        input_sensors.button_b.press = button_b;
#endif

#if BUTTON_JOYSTICK_ENABLED
        auto* button_joystick = new BUTTON_CLASS(PIN_BUTTON_JOYSTICK, INPUT_PULLUP, BUTTON_JOYSTICK_INVERT);
        input_sensors.joystick.press = button_joystick;
#endif

#if BUTTON_MENU_ENABLED
        auto* button_menu = new BUTTON_CLASS(PIN_BUTTON_MENU, INPUT_PULLUP, BUTTON_MENU_INVERT);
#endif

#if BUTTON_CALIBRATE_ENABLED
        auto* button_calibrate = new BUTTON_CLASS(PIN_BUTTON_CALIBRATE, INPUT_PULLUP, BUTTON_CALIBRATE_INVERT);
        input_sensors.button_calibrate.press = button_calibrate;
#endif

#if GESTURE_TRIGGER_ENABLED && FINGER_INDEX_ENABLED
        auto* trigger = new Body::Hands::Input::TriggerGesture(index_curl_sensor, GESTURE_TRIGGER_THRESHOLD);
        input_sensors.trigger.press = trigger;
#elif BUTTON_TRIGGER_ENABLED
        auto trigger = new BUTTON_CLASS(PIN_BUTTON_TRIGGER, INPUT_PULLUP, BUTTON_TRIGGER_INVERT);
#endif

#if GESTURE_GRAB_ENABLED && FINGER_INDEX_ENABLED && FINGER_MIDDLE_ENABLED && FINGER_RING_ENABLED && FINGER_PINKY_ENABLED
        auto* grab = new Body::Hands::Input::GrabGesture(
          Body::Hands::Input::GrabGesture::Fingers{ .index = index_curl_sensor,
                                                    .middle = middle_curl_sensor,
                                                    .ring = ring_curl_sensor,
                                                    .pinky = pinky_curl_sensor },
          GESTURE_GRAB_THRESHOLD
        );
        input_sensors.grab.press = grab;
#elif BUTTON_GRAB_ENABLED
        auto* grab = new BUTTON_CLASS(PIN_BUTTON_GRAB, INPUT_PULLUP, BUTTON_GRAB_INVERT);
#endif

#if GESTURE_PINCH_ENABLED && FINGER_THUMB_ENABLED && FINGER_INDEX_ENABLED
        auto* pinch = new Body::Hands::Input::PinchGesture(
          Body::Hands::Input::PinchGesture::Fingers{ .thumb = thumb_curl_sensor, .index = index_curl_sensor },
          GESTURE_PINCH_THRESHOLD
        );
        input_sensors.pinch.press = pinch;
#elif BUTTON_PINCH_ENABLED
        auto* pinch = new BUTTON_CLASS(PIN_BUTTON_PINCH, INPUT_PULLUP, BUTTON_PINCH_INVERT);
#endif

        return input_sensors;
    }

    [[nodiscard]] auto createFfbOutputs() -> OutputWriters
    {
        OutputWriters output_writers;

#if FFB_THUMB_ENABLED
        auto* thumb_ffb_output = new ::SenseShift::Arduino::Output::ServoOutput(PIN_FFB_THUMB);
        output_writers.ffb.thumb = thumb_ffb_output;
#endif

#if FFB_INDEX_ENABLED
        auto* index_ffb_output = new ::SenseShift::Arduino::Output::ServoOutput(PIN_FFB_INDEX);
        output_writers.ffb.index = index_ffb_output;
#endif

#if FFB_MIDDLE_ENABLED
        auto* middle_ffb_output = new ::SenseShift::Arduino::Output::ServoOutput(PIN_FFB_MIDDLE);
        output_writers.ffb.middle = middle_ffb_output;
#endif

#if FFB_RING_ENABLED
        auto* ring_ffb_output = new ::SenseShift::Arduino::Output::ServoOutput(PIN_FFB_RING);
        output_writers.ffb.ring = ring_ffb_output;
#endif

#if FFB_PINKY_ENABLED
        auto* pinky_ffb_output = new ::SenseShift::Arduino::Output::ServoOutput(PIN_FFB_PINKY);
        output_writers.ffb.pinky = pinky_ffb_output;
#endif

        return output_writers;
    }

    /**
     * Setup the transport for the OpenGloves interface.
     */
    [[nodiscard]] auto createTransport() -> ITransport*
    {
#if OPENGLOVES_COMMUNICATION == OPENGLOVES_COMM_SERIAL // Serial
        auto* pSerial = &SERIAL_PORT;
        pSerial->begin(SERIAL_BAUDRATE);
        return new StreamTransport(pSerial);
#elif (OPENGLOVES_COMMUNICATION == OPENGLOVES_COMM_BTSERIAL) \
  || (OPENGLOVES_COMMUNICATION == OPENGLOVES_COMM_BLESERIAL) // Bluetooth
        std::string name;
#ifdef BTSERIAL_NAME
        name = BTSERIAL_NAME;
#else
        char suffix[4];
        snprintf(suffix, 4, "%04X", (uint16_t) (ESP.getEfuseMac() >> 32));
        name = BTSERIAL_PREFIX + std::string(suffix);

        log_i("Generated Bluetooth name: %s", name.c_str());
#endif

#if OPENGLOVES_COMMUNICATION == OPENGLOVES_COMM_BTSERIAL    // Bluetooth Classic
        BluetoothSerial* pBtSerial = new BluetoothSerial();
        pBtSerial->begin(name.c_str());
        return new BluetoothSerialTransport(*pBtSerial);
#elif OPENGLOVES_COMMUNICATION == OPENGLOVES_COMM_BLESERIAL // Bluetooth Low Energy
        BLESerial* pBleSerial = new BLESerial();
        pBleSerial->begin(name.c_str());
        return new BLESerialTransport(*pBleSerial);
#endif
#else // Fallback
#error "Unsupported communication type"
        return nullptr;
#endif
    }
} // namespace SenseShift::OpenGloves::AutoConfig
