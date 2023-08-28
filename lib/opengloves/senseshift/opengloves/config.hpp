#pragma once

#include <senseshift/opengloves/constants.hpp>

#pragma region Communication

#ifndef OPENGLOVES_COMMUNCATION
#define OPENGLOVES_COMMUNCATION OPENGLOVES_COMM_SERIAL
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
#define CALIBRATION_CURL ::SenseShift::Calibration::MinMaxCalibrator<uint16_t, 0, ANALOG_MAX>
#endif
#ifndef CALIBRATION_SPLAY
#define CALIBRATION_SPLAY ::SenseShift::Calibration::MinMaxCalibrator<uint16_t, 0, ANALOG_MAX>
#endif

#ifndef CALIBRATION_DURATION
#define CALIBRATION_DURATION 2000 // duration in milliseconds
#endif

#pragma endregion

#pragma region Fingers

// clang-format off
#define FINGER_THUMB_ENABLED  (defined(PIN_FINGER_THUMB)  && (PIN_FINGER_THUMB  != -1))
#define FINGER_INDEX_ENABLED  (defined(PIN_FINGER_INDEX)  && (PIN_FINGER_INDEX  != -1))
#define FINGER_MIDDLE_ENABLED (defined(PIN_FINGER_MIDDLE) && (PIN_FINGER_MIDDLE != -1))
#define FINGER_RING_ENABLED   (defined(PIN_FINGER_RING)   && (PIN_FINGER_RING   != -1))
#define FINGER_PINKY_ENABLED  (defined(PIN_FINGER_PINKY)  && (PIN_FINGER_PINKY  != -1))
// clang-format on

#define FINGER_CLASS(type, curl_pin, curl_invert, curl_calib)                  \
    FingerSensor(                                                              \
      new ::SenseShift::Input::CalibratedSensor<uint16_t>(                     \
        new ::SenseShift::Arduino::Input::AnalogSensor<curl_invert>(curl_pin), \
        new curl_calib()                                                       \
      ),                                                                       \
      type                                                                     \
    )

// clang-format off
#define FINGER_THUMB_SPLAY  (FINGER_THUMB_ENABLED  && defined(PIN_FINGER_THUMB_SPLAY)  && (PIN_FINGER_THUMB_SPLAY  != -1))
#define FINGER_INDEX_SPLAY  (FINGER_INDEX_ENABLED  && defined(PIN_FINGER_INDEX_SPLAY)  && (PIN_FINGER_INDEX_SPLAY  != -1))
#define FINGER_MIDDLE_SPLAY (FINGER_MIDDLE_ENABLED && defined(PIN_FINGER_MIDDLE_SPLAY) && (PIN_FINGER_MIDDLE_SPLAY != -1))
#define FINGER_RING_SPLAY   (FINGER_RING_ENABLED   && defined(PIN_FINGER_RING_SPLAY)   && (PIN_FINGER_RING_SPLAY   != -1))
#define FINGER_PINKY_SPLAY  (FINGER_PINKY_ENABLED  && defined(PIN_FINGER_PINKY_SPLAY)  && (PIN_FINGER_PINKY_SPLAY  != -1))
// clang-format on

#define FINGER_SPLAY_CLASS(type, curl_pin, curl_invert, curl_calib, splay_pin, splay_invert, splay_calib) \
    FingerSensor(                                                                                         \
      new ::SenseShift::Input::CalibratedSensor<uint16_t>(                                                \
        new ::SenseShift::Arduino::Input::AnalogSensor<curl_invert>(curl_pin),                            \
        new curl_calib()                                                                                  \
      ),                                                                                                  \
      new ::SenseShift::Input::CalibratedSensor<uint16_t>(                                                \
        new ::SenseShift::Arduino::Input::AnalogSensor<splay_invert>(splay_pin),                          \
        new splay_calib()                                                                                 \
      ),                                                                                                  \
      type                                                                                                \
    )

#pragma endregion

#pragma region Joysticks

#define JOYSTICK_ENABLED \
    (defined(PIN_JOYSTICK_X) && defined(PIN_JOYSTICK_Y) && (PIN_JOYSTICK_X != -1) && (PIN_JOYSTICK_Y != -1))

#define JOYSTICK_CLASS(type, pin, invert, deadzone)                  \
    StringEncodedMemoizedSensor<uint16_t>(                           \
      new ::SenseShift::Input::JoystickAxisSensor<uint16_t>(         \
        new ::SenseShift::Arduino::Input::AnalogSensor<invert>(pin), \
        deadzone                                                     \
      ),                                                             \
      type                                                           \
    )

#pragma endregion

#pragma region Buttons

#define BUTTON_A_ENABLED (defined(PIN_BUTTON_A) && (PIN_BUTTON_A != -1))
#define BUTTON_B_ENABLED (defined(PIN_BUTTON_B) && (PIN_BUTTON_B != -1))
#define BUTTON_MENU_ENABLED (defined(PIN_BUTTON_MENU) && (PIN_BUTTON_MENU != -1))
#define BUTTON_JOYSTICK_ENABLED (JOYSTICK_ENABLED && defined(PIN_BUTTON_JOYSTICK) && (PIN_BUTTON_JOYSTICK != -1))
#define BUTTON_CALIBRATE_ENABLED (defined(PIN_BUTTON_CALIBRATE) && (PIN_BUTTON_CALIBRATE != -1))
#define BUTTON_TRIGGER_ENABLED (!GESTURE_TRIGGER_ENABLED && defined(PIN_BUTTON_TRIGGER) && (PIN_BUTTON_TRIGGER != -1))
#define BUTTON_GRAB_ENABLED (!GESTURE_GRAB_ENABLED && defined(PIN_BUTTON_GRAB) && (PIN_BUTTON_GRAB != -1))
#define BUTTON_PINCH_ENABLED (!GESTURE_PINCH_ENABLED && defined(PIN_BUTTON_PINCH) && (PIN_BUTTON_PINCH != -1))

#define BUTTON_CLASS(type, pin, invert) \
    StringEncodedMemoizedSensor<bool>(new ::SenseShift::Arduino::Input::DigitalSensor<invert>(pin), type)

#pragma endregion

#pragma region Gestures

#ifndef GESTURE_TRIGGER_THRESHOLD
#define GESTURE_TRIGGER_THRESHOLD (ANALOG_MAX / 2)
#endif

#ifndef GESTURE_GRAB_THRESHOLD
#define GESTURE_GRAB_THRESHOLD (ANALOG_MAX / 2)
#endif

#ifndef GESTURE_PINCH_THRESHOLD
#define GESTURE_PINCH_THRESHOLD (ANALOG_MAX / 2)
#endif

#define GESTURE_CLASS(type, sensor) StringEncodedMemoizedSensor<bool>(sensor, type)

#pragma endregion

#pragma region FFB

#define FFB_THUMB_ENABLED (defined(PIN_FFB_THUMB) && (PIN_FFB_THUMB != -1))
#define FFB_INDEX_ENABLED (defined(PIN_FFB_INDEX) && (PIN_FFB_INDEX != -1))
#define FFB_MIDDLE_ENABLED (defined(PIN_FFB_MIDDLE) && (PIN_FFB_MIDDLE != -1))
#define FFB_RING_ENABLED (defined(PIN_FFB_RING) && (PIN_FFB_RING != -1))
#define FFB_PINKY_ENABLED (defined(PIN_FFB_PINKY) && (PIN_FFB_PINKY != -1))

#define FFB_ENABLED \
    (FFB_THUMB_ENABLED || FFB_INDEX_ENABLED || FFB_MIDDLE_ENABLED || FFB_RING_ENABLED || FFB_PINKY_ENABLED)

#pragma endregion

#ifndef UPDATE_RATE
#define UPDATE_RATE 90
#endif
