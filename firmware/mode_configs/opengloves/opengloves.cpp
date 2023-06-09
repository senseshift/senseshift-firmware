#include <utility.hpp>
#include <calibration.hpp>
#include <sensor.hpp>
#include <sensor/analog.hpp>
#include <sensor/joystick.hpp>
#include <sensor/og_finger.hpp>

#define CALIBRATION_CURL OH::MinMaxCalibrator<uint16_t, 0, ANALOG_MAX>

#define FINGER_THUMB_ENABLED (PIN_FINGER_THUMB != -1)
#define FINGER_INDEX_ENABLED (PIN_FINGER_INDEX != -1)
#define FINGER_MIDDLE_ENABLED (PIN_FINGER_MIDDLE != -1)
#define FINGER_RING_ENABLED (PIN_FINGER_RING != -1)
#define FINGER_PINKY_ENABLED (PIN_FINGER_PINKY != -1)
#define FINGER_COUNT (FINGER_THUMB_ENABLED + FINGER_INDEX_ENABLED + FINGER_MIDDLE_ENABLED + FINGER_RING_ENABLED + FINGER_PINKY_ENABLED)

#define JOYSTICK_ENABLED (PIN_JOYSTICK_X != -1 && PIN_JOYSTICK_Y != -1)
#define JOYSTICK_COUNT (JOYSTICK_ENABLED ? 2 : 0)

using namespace OpenGloves;

#pragma region FingerSensor

#if FINGER_THUMB_ENABLED
  auto* fingerThumb = new FingerSensor(new OH::AnalogSensor<FINGER_THUMB_INVERT>(PIN_FINGER_THUMB), new CALIBRATION_CURL(), EncodedInput::Type::THUMB);
#endif

#if FINGER_INDEX_ENABLED
  auto* fingerIndex = new FingerSensor(new OH::AnalogSensor<FINGER_INDEX_INVERT>(PIN_FINGER_INDEX), new CALIBRATION_CURL(), EncodedInput::Type::INDEX);
#endif

#if FINGER_MIDDLE_ENABLED
  auto* fingerMiddle = new FingerSensor(new OH::AnalogSensor<FINGER_MIDDLE_INVERT>(PIN_FINGER_MIDDLE), new CALIBRATION_CURL(), EncodedInput::Type::MIDDLE);
#endif

#if FINGER_RING_ENABLED
  auto* fingerRing = new FingerSensor(new OH::AnalogSensor<FINGER_RING_INVERT>(PIN_FINGER_RING), new CALIBRATION_CURL(), EncodedInput::Type::RING);
#endif

#if FINGER_PINKY_ENABLED
  auto* fingerPinky = new FingerSensor(new OH::AnalogSensor<FINGER_PINKY_INVERT>(PIN_FINGER_PINKY), new CALIBRATION_CURL(), EncodedInput::Type::PINKY);
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
  new OH::MemoizedSensor<uint16_t>(new OH::JoystickAxisSensor<uint16_t>(new OH::AnalogSensor<JOYSTICK_X_INVERT>(PIN_JOYSTICK_X), JOYSTICK_DEADZONE)),
  new OH::MemoizedSensor<uint16_t>(new OH::JoystickAxisSensor<uint16_t>(new OH::AnalogSensor<JOYSTICK_Y_INVERT>(PIN_JOYSTICK_Y), JOYSTICK_DEADZONE)),
#endif
};

void setupMode() {
  for (int i = 0; i < FINGER_COUNT; i++) {
    fingers[i]->setup();
  }

  for (int i = 0; i < JOYSTICK_COUNT; i++) {
    joystick[i]->setup();
  }
}

void loopMode() {
  for (int i = 0; i < FINGER_COUNT; i++) {
    // fingers[i]->readValue();
  }

  for (int i = 0; i < JOYSTICK_COUNT; i++) {
    joystick[i]->updateValue();
  }
}
