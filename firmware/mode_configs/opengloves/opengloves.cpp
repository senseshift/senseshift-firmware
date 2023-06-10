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

#define JOYSTICK_CLASS(type, pin, invert, deadzone) new OH::MemoizedSensor<uint16_t>(new OH::JoystickAxisSensor<uint16_t>(new OH::AnalogSensor<invert>(pin), deadzone))

using namespace OpenGloves;

#pragma region FingerSensor

#if FINGER_THUMB_ENABLED
  auto* fingerThumb = FINGER_CLASS(IEncodedInput::Type::THUMB, PIN_FINGER_THUMB, FINGER_THUMB_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_INDEX_ENABLED
  auto* fingerIndex = FINGER_CLASS(IEncodedInput::Type::INDEX, PIN_FINGER_INDEX, FINGER_INDEX_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_MIDDLE_ENABLED
  auto* fingerMiddle = FINGER_CLASS(IEncodedInput::Type::MIDDLE, PIN_FINGER_MIDDLE, FINGER_MIDDLE_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_RING_ENABLED
  auto* fingerRing = FINGER_CLASS(IEncodedInput::Type::RING, PIN_FINGER_RING, FINGER_RING_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_PINKY_ENABLED
  auto* fingerPinky = FINGER_CLASS(IEncodedInput::Type::PINKY, PIN_FINGER_PINKY, FINGER_PINKY_INVERT, CALIBRATION_CURL);
#endif

FingerSensor* fingers[FINGER_COUNT] = {
#if FINGER_THUMB_ENABLED
  fingerThumb,
#endif
#if FINGER_INDEX_ENABLED
  fingerIndex,
#endif
#if FINGER_MIDDLE_ENABLED
  fingerMiddle,
#endif
#if FINGER_RING_ENABLED
  fingerRing,
#endif
#if FINGER_PINKY_ENABLED
  fingerPinky,
#endif
};

#pragma endregion

OH::MemoizedSensor<uint16_t>* joystick[JOYSTICK_COUNT] = {
#if JOYSTICK_ENABLED
  JOYSTICK_CLASS(IEncodedInput::Type::JOY_X, PIN_JOYSTICK_X, JOYSTICK_X_INVERT, JOYSTICK_DEADZONE),
  JOYSTICK_CLASS(IEncodedInput::Type::JOY_Y, PIN_JOYSTICK_Y, JOYSTICK_Y_INVERT, JOYSTICK_DEADZONE),
#endif
};

std::vector<IEncodedSensor*> inputs = std::vector<IEncodedSensor*>();

auto communication = new SerialCommunication(&Serial);

void setupMode() {
  communication->setup();

  for (size_t i = 0; i < FINGER_COUNT; i++) {
    auto* finger = fingers[i];
    inputs.push_back(finger);
    finger->setup();
  }

  for (size_t i = 0; i < JOYSTICK_COUNT; i++) {
    joystick[i]->setup();
  }
}

void loopMode() {
  // update all sensor values
  for (size_t i = 0; i < inputs.size(); i++) {
    inputs[i]->updateValue();
  }

  // send all sensor values
  communication->send(inputs);
}
