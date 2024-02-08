#pragma once

#include <senseshift/opengloves/opengloves.hpp>

#ifdef ARDUINO
#include <senseshift/opengloves/transport/stream.hpp>
#endif

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

#define DEFINE_FINGER(NAME, CURL_PIN, CURL_INVERT, CURL_CALIB)                                                   \
    auto* NAME##_sensor = new ::SenseShift::Input::SimpleSensorDecorator(                                        \
      new ::SenseShift::Arduino::Input::AnalogSimpleSensor<CURL_INVERT>(CURL_PIN)                                \
    );                                                                                                           \
    NAME##_sensor->addFilters({ new ::SenseShift::Input::Filter::ExponentialMovingAverageFilter<float>(0.8F) }); \
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

#define DEFINE_JOYSTICK_AXIS(NAME, PIN, INVERT, DEADZONE)                                                              \
    auto NAME##_sensor =                                                                                               \
      new ::SenseShift::Input::SimpleSensorDecorator(new ::SenseShift::Arduino::Input::AnalogSimpleSensor<INVERT>(PIN) \
      );                                                                                                               \
    NAME##_sensor->addFilters({ new ::SenseShift::Input::Filter::ExponentialMovingAverageFilter<float>(0.8F),          \
                                new ::SenseShift::Input::Filter::CenterDeadzoneFilter(DEADZONE) });

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

#define BUTTON_CLASS(PIN, INVERT) SimpleSensorDecorator(new DigitalSimpleSensor<INVERT>(PIN));

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

#define FFB_THUMB_ENABLED (defined(PIN_FFB_THUMB) && (PIN_FFB_THUMB != -1))
#define FFB_INDEX_ENABLED (defined(PIN_FFB_INDEX) && (PIN_FFB_INDEX != -1))
#define FFB_MIDDLE_ENABLED (defined(PIN_FFB_MIDDLE) && (PIN_FFB_MIDDLE != -1))
#define FFB_RING_ENABLED (defined(PIN_FFB_RING) && (PIN_FFB_RING != -1))
#define FFB_PINKY_ENABLED (defined(PIN_FFB_PINKY) && (PIN_FFB_PINKY != -1))

#define FFB_ENABLED \
    (FFB_THUMB_ENABLED || FFB_INDEX_ENABLED || FFB_MIDDLE_ENABLED || FFB_RING_ENABLED || FFB_PINKY_ENABLED)

#ifndef UPDATE_RATE
#define UPDATE_RATE 90
#endif

namespace SenseShift::OpenGloves::AutoConfig {

    /**
     * Setup the transport for the OpenGloves interface.
     */
    [[nodiscard]] ITransport* setupTransport(void)
    {
#if OPENGLOVES_COMMUNCATION == OPENGLOVES_COMM_SERIAL // Serial
        auto* pSerial = &SERIAL_PORT;
        pSerial->begin(SERIAL_BAUDRATE);
        return new StreamTransport(pSerial);
#elif (OPENGLOVES_COMMUNCATION == OPENGLOVES_COMM_BTSERIAL) \
  || (OPENGLOVES_COMMUNCATION == OPENGLOVES_COMM_BLESERIAL) // Bluetooth Serial
        std::string name;
#ifdef BTSERIAL_NAME
        name = BTSERIAL_NAME;
#else
        char suffix[4];
        snprintf(suffix, 4, "%04X", (uint16_t) (ESP.getEfuseMac() >> 32));
        name = BTSERIAL_PREFIX + std::string(suffix);

        log_i("Generated Bluetooth name: %s", name.c_str());
#endif

#if OPENGLOVES_COMMUNCATION == OPENGLOVES_COMM_BTSERIAL    // Bluetooth Classic
        BluetoothSerial* pBtSerial = new BluetoothSerial();
        pBtSerial->begin(name.c_str());
        return new BluetoothSerialTransport(*pBtSerial);
#elif OPENGLOVES_COMMUNCATION == OPENGLOVES_COMM_BLESERIAL // Bluetooth Low Energy
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
