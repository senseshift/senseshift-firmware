#include <utility.hpp>
#include <calibration.hpp>
#include <sensor.hpp>
#include <sensor/analog.hpp>
#include <sensor/joystick.hpp>
#include <sensor/og_finger.hpp>
#include <og_serial_commmunications.hpp>

#ifndef CALIBRATION_CURL
#define CALIBRATION_CURL OH::MinMaxCalibrator<uint16_t, 0, ANALOG_MAX>
#endif

#define FINGER_THUMB_ENABLED (PIN_FINGER_THUMB != -1)
#define FINGER_INDEX_ENABLED (PIN_FINGER_INDEX != -1)
#define FINGER_MIDDLE_ENABLED (PIN_FINGER_MIDDLE != -1)
#define FINGER_RING_ENABLED (PIN_FINGER_RING != -1)
#define FINGER_PINKY_ENABLED (PIN_FINGER_PINKY != -1)
#define FINGER_COUNT (FINGER_THUMB_ENABLED + FINGER_INDEX_ENABLED + FINGER_MIDDLE_ENABLED + FINGER_RING_ENABLED + FINGER_PINKY_ENABLED)

#define FINGER_CLASS(type, pin, invert, calib) new FingerSensor(new OH::CalibratedSensor<uint16_t>(new OH::AnalogSensor<invert>(pin), new calib()), type)

#define JOYSTICK_ENABLED (PIN_JOYSTICK_X != -1 && PIN_JOYSTICK_Y != -1)
#define JOYSTICK_COUNT (JOYSTICK_ENABLED ? 2 : 0)

#define JOYSTICK_CLASS(type, pin, invert, deadzone) new StringEncodedMemoizedSensor<uint16_t>(new OH::JoystickAxisSensor<uint16_t>(new OH::AnalogSensor<invert>(pin), deadzone), type)

using namespace OpenGloves;

#pragma region FingerSensor

#if FINGER_THUMB_ENABLED
  auto* fingerThumbCurl = FINGER_CLASS(IEncodedInput::Type::THUMB, PIN_FINGER_THUMB, FINGER_THUMB_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_INDEX_ENABLED
  auto* fingerIndexCurl = FINGER_CLASS(IEncodedInput::Type::INDEX, PIN_FINGER_INDEX, FINGER_INDEX_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_MIDDLE_ENABLED
  auto* fingerMiddleCurl = FINGER_CLASS(IEncodedInput::Type::MIDDLE, PIN_FINGER_MIDDLE, FINGER_MIDDLE_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_RING_ENABLED
  auto* fingerRingCurl = FINGER_CLASS(IEncodedInput::Type::RING, PIN_FINGER_RING, FINGER_RING_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_PINKY_ENABLED
  auto* fingerPinkyCurl = FINGER_CLASS(IEncodedInput::Type::PINKY, PIN_FINGER_PINKY, FINGER_PINKY_INVERT, CALIBRATION_CURL);
#endif

FingerSensor* fingers[FINGER_COUNT] = {
#if FINGER_THUMB_ENABLED
  fingerThumbCurl,
#endif
#if FINGER_INDEX_ENABLED
  fingerIndexCurl,
#endif
#if FINGER_MIDDLE_ENABLED
  fingerMiddleCurl,
#endif
#if FINGER_RING_ENABLED
  fingerRingCurl,
#endif
#if FINGER_PINKY_ENABLED
  fingerPinkyCurl,
#endif
};

#pragma endregion

StringEncodedMemoizedSensor<uint16_t>* joystick[JOYSTICK_COUNT] = {
#if JOYSTICK_ENABLED
  JOYSTICK_CLASS(IEncodedInput::Type::JOY_X, PIN_JOYSTICK_X, JOYSTICK_X_INVERT, JOYSTICK_DEADZONE),
  JOYSTICK_CLASS(IEncodedInput::Type::JOY_Y, PIN_JOYSTICK_Y, JOYSTICK_Y_INVERT, JOYSTICK_DEADZONE),
#endif
};

std::vector<IStringEncodedSensor*> inputs = std::vector<IStringEncodedSensor*>();
std::vector<OH::ICalibrated*> calibrated = std::vector<OH::ICalibrated*>();

auto communication = new SerialCommunication(&Serial);

void setupMode() {
  for (size_t i = 0; i < FINGER_COUNT; i++) {
    auto* finger = fingers[i];
    finger->setup();

    #if defined(CALIBRATION_ALWAYS_CALIBRATE) && CALIBRATION_ALWAYS_CALIBRATE
      finger->enableCalibration();
    #endif

    inputs.push_back(finger);
    calibrated.push_back(finger);
  }

  for (size_t i = 0; i < JOYSTICK_COUNT; i++) {
    auto* axis = joystick[i];
    axis->setup();

    inputs.push_back(axis);
  }

  communication->setup();
}

void loopMode() {
  // update all sensor values
  for (size_t i = 0; i < inputs.size(); i++) {
    auto* input = inputs[i];
    input->updateValue();
  }

  // send all sensor values
  communication->send(inputs);
}
