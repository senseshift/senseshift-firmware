#include <utility.hpp>
#include <sensor.hpp>
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
  auto* fingerThumb = new FingerSensor(PIN_FINGER_THUMB, FINGER_THUMB_INVERT, new CALIBRATION_CURL());
#endif

#if FINGER_INDEX_ENABLED
  auto* fingerIndex = new FingerSensor(PIN_FINGER_INDEX, FINGER_INDEX_INVERT, new CALIBRATION_CURL());
#endif

#if FINGER_MIDDLE_ENABLED
  auto* fingerMiddle = new FingerSensor(PIN_FINGER_MIDDLE, FINGER_MIDDLE_INVERT, new CALIBRATION_CURL());
#endif

#if FINGER_RING_ENABLED
  auto* fingerRing = new FingerSensor(PIN_FINGER_RING, FINGER_RING_INVERT, new CALIBRATION_CURL());
#endif

#if FINGER_PINKY_ENABLED
  auto* fingerPinky = new FingerSensor(PIN_FINGER_PINKY, FINGER_PINKY_INVERT, new CALIBRATION_CURL());
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

OH::JoystickAxisSensor* joystick[JOYSTICK_COUNT] = {
#if JOYSTICK_ENABLED
  new OH::JoystickAxisSensor(PIN_JOYSTICK_X, JOYSTICK_DEADZONE, JOYSTICK_X_INVERT),
  new OH::JoystickAxisSensor(PIN_JOYSTICK_Y, JOYSTICK_DEADZONE, JOYSTICK_Y_INVERT),
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

}
