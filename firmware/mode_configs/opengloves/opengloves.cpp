#include <utility.hpp>
#include <calibration.hpp>
#include <sensor.hpp>
#include <sensor/analog.hpp>
#include <sensor/joystick.hpp>

#include <sensor/og_finger.hpp>

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

#define INPUTS_COUNT (FINGER_COUNT)

using namespace OpenGloves;

#pragma region FingerSensor

#if FINGER_THUMB_ENABLED
  auto* fingerThumb = FINGER_CLASS(EncodedInput::Type::THUMB, PIN_FINGER_THUMB, FINGER_THUMB_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_INDEX_ENABLED
  auto* fingerIndex = FINGER_CLASS(EncodedInput::Type::INDEX, PIN_FINGER_INDEX, FINGER_INDEX_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_MIDDLE_ENABLED
  auto* fingerMiddle = FINGER_CLASS(EncodedInput::Type::MIDDLE, PIN_FINGER_MIDDLE, FINGER_MIDDLE_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_RING_ENABLED
  auto* fingerRing = FINGER_CLASS(EncodedInput::Type::RING, PIN_FINGER_RING, FINGER_RING_INVERT, CALIBRATION_CURL);
#endif

#if FINGER_PINKY_ENABLED
  auto* fingerPinky = FINGER_CLASS(EncodedInput::Type::PINKY, PIN_FINGER_PINKY, FINGER_PINKY_INVERT, CALIBRATION_CURL);
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
  JOYSTICK_CLASS(EncodedInput::Type::JOY_X, PIN_JOYSTICK_X, JOYSTICK_X_INVERT, JOYSTICK_DEADZONE),
  JOYSTICK_CLASS(EncodedInput::Type::JOY_Y, PIN_JOYSTICK_Y, JOYSTICK_Y_INVERT, JOYSTICK_DEADZONE),
#endif
};

IEncodedSensor* inputs[INPUTS_COUNT];
char* encoded_output_string;

void setupMode() {
  size_t currentSensor = 0;

  for (size_t i = 0; i < FINGER_COUNT; i++) {
    auto* finger = fingers[i];
    inputs[currentSensor++] = finger;
    finger->setup();
    finger->enableCalibration();
  }

  for (size_t i = 0; i < JOYSTICK_COUNT; i++) {
    // sensors[currentSensor++] = joystick[i];
    joystick[i]->setup();
  }

  int string_size = 0;
  for(size_t i = 0; i < INPUTS_COUNT; i++) {
    string_size += inputs[i]->getEncodedSize();
  }

  // Add 1 for new line and 1 for the null terminator.
  encoded_output_string = new char[string_size + 1 + 1];

  Serial.begin(115200);
}

int encodeAll(char* output, IEncodedSensor* encoders[], size_t count) {
  int offset = 0;
  // Loop over all of the encoders and encode them to the output string.
  for (size_t i = 0; i < count; i++) {
    // The offset is the total charecters already added to the string.
    offset += encoders[i]->encode(output+offset);
  }

  // Add a new line to the end of the encoded string.
  output[offset++] = '\n';
  output[offset] = '\0';

  return offset;
}

void loopMode() {
  // update all sensor values
  for (int i = 0; i < INPUTS_COUNT; i++) {
    auto* sensor = inputs[i];
    sensor->updateValue();
  }

  // encode all sensor values
  encodeAll(encoded_output_string, inputs, INPUTS_COUNT);
  Serial.print(encoded_output_string);
}
