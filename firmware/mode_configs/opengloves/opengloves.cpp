#include <og_constants.hpp>
#include <og_serial_communication.hpp>
#include <opengloves_task.hpp>
#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/input/sensor/digital.hpp>
#include <senseshift/arduino/output/servo.hpp>
#include <senseshift/calibration.hpp>
#include <senseshift/input/sensor.hpp>
#include <senseshift/input/sensor/joystick.hpp>
#include <senseshift/utility.hpp>
#include <sensor/og_finger.hpp>
#include <sensor/og_gesture.hpp>

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

#define FINGER_THUMB_ENABLED (defined(PIN_FINGER_THUMB) && (PIN_FINGER_THUMB != -1))
#define FINGER_INDEX_ENABLED (defined(PIN_FINGER_INDEX) && (PIN_FINGER_INDEX != -1))
#define FINGER_MIDDLE_ENABLED (defined(PIN_FINGER_MIDDLE) && (PIN_FINGER_MIDDLE != -1))
#define FINGER_RING_ENABLED (defined(PIN_FINGER_RING) && (PIN_FINGER_RING != -1))
#define FINGER_PINKY_ENABLED (defined(PIN_FINGER_PINKY) && (PIN_FINGER_PINKY != -1))
#define FINGER_CLASS(type, curl_pin, curl_invert, curl_calib)                  \
    FingerSensor(                                                              \
      new ::SenseShift::Input::CalibratedSensor<uint16_t>(                     \
        new ::SenseShift::Arduino::Input::AnalogSensor<curl_invert>(curl_pin), \
        new curl_calib()                                                       \
      ),                                                                       \
      type                                                                     \
    )
#define FINGER_THUMB_SPLAY (FINGER_THUMB_ENABLED && defined(PIN_FINGER_THUMB_SPLAY) && (PIN_FINGER_THUMB_SPLAY != -1))
#define FINGER_INDEX_SPLAY (FINGER_INDEX_ENABLED && defined(PIN_FINGER_INDEX_SPLAY) && (PIN_FINGER_INDEX_SPLAY != -1))
#define FINGER_MIDDLE_SPLAY \
    (FINGER_MIDDLE_ENABLED && defined(PIN_FINGER_MIDDLE_SPLAY) && (PIN_FINGER_MIDDLE_SPLAY != -1))
#define FINGER_RING_SPLAY (FINGER_RING_ENABLED && defined(PIN_FINGER_RING_SPLAY) && (PIN_FINGER_RING_SPLAY != -1))
#define FINGER_PINKY_SPLAY (FINGER_PINKY_ENABLED && defined(PIN_FINGER_PINKY_SPLAY) && (PIN_FINGER_PINKY_SPLAY != -1))
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

#ifndef UPDATE_RATE
#define UPDATE_RATE 90
#endif

#define FFB_THUMB_ENABLED (defined(PIN_FFB_THUMB) && (PIN_FFB_THUMB != -1))
#define FFB_INDEX_ENABLED (defined(PIN_FFB_INDEX) && (PIN_FFB_INDEX != -1))
#define FFB_MIDDLE_ENABLED (defined(PIN_FFB_MIDDLE) && (PIN_FFB_MIDDLE != -1))
#define FFB_RING_ENABLED (defined(PIN_FFB_RING) && (PIN_FFB_RING != -1))
#define FFB_PINKY_ENABLED (defined(PIN_FFB_PINKY) && (PIN_FFB_PINKY != -1))

#define FFB_ENABLED \
    (FFB_THUMB_ENABLED || FFB_INDEX_ENABLED || FFB_MIDDLE_ENABLED || FFB_RING_ENABLED || FFB_PINKY_ENABLED)

using namespace OpenGloves;

HandSensors handSensors = {
#if FINGER_THUMB_SPLAY
    .thumb = FINGER_SPLAY_CLASS(
      IEncodedInput::Type::THUMB,
      PIN_FINGER_THUMB,
      FINGER_THUMB_INVERT,
      CALIBRATION_CURL,
      PIN_FINGER_THUMB_SPLAY,
      FINGER_THUMB_SPLAY_INVERT,
      CALIBRATION_SPLAY
    ),
#elif FINGER_THUMB_ENABLED
    .thumb = FINGER_CLASS(IEncodedInput::Type::THUMB, PIN_FINGER_THUMB, FINGER_THUMB_INVERT, CALIBRATION_CURL),
#endif

#if FINGER_INDEX_SPLAY
    .index = FINGER_SPLAY_CLASS(
      IEncodedInput::Type::INDEX,
      PIN_FINGER_INDEX,
      FINGER_INDEX_INVERT,
      CALIBRATION_CURL,
      PIN_FINGER_INDEX_SPLAY,
      FINGER_INDEX_SPLAY_INVERT,
      CALIBRATION_SPLAY
    ),
#elif FINGER_INDEX_ENABLED
    .index = FINGER_CLASS(IEncodedInput::Type::INDEX, PIN_FINGER_INDEX, FINGER_INDEX_INVERT, CALIBRATION_CURL),
#endif

#if FINGER_MIDDLE_SPLAY
    .middle = FINGER_SPLAY_CLASS(
      IEncodedInput::Type::MIDDLE,
      PIN_FINGER_MIDDLE,
      FINGER_MIDDLE_INVERT,
      CALIBRATION_CURL,
      PIN_FINGER_MIDDLE_SPLAY,
      FINGER_MIDDLE_SPLAY_INVERT,
      CALIBRATION_SPLAY
    ),
#elif FINGER_MIDDLE_ENABLED
    .middle = FINGER_CLASS(IEncodedInput::Type::MIDDLE, PIN_FINGER_MIDDLE, FINGER_MIDDLE_INVERT, CALIBRATION_CURL),
#endif

#if FINGER_RING_SPLAY
    .ring = FINGER_SPLAY_CLASS(
      IEncodedInput::Type::RING,
      PIN_FINGER_RING,
      FINGER_RING_INVERT,
      CALIBRATION_CURL,
      PIN_FINGER_RING_SPLAY,
      FINGER_RING_SPLAY_INVERT,
      CALIBRATION_SPLAY
    ),
#elif FINGER_RING_ENABLED
    .ring = FINGER_CLASS(IEncodedInput::Type::RING, PIN_FINGER_RING, FINGER_RING_INVERT, CALIBRATION_CURL),
#endif

#if FINGER_PINKY_SPLAY
    .pinky = FINGER_SPLAY_CLASS(
      IEncodedInput::Type::PINKY,
      PIN_FINGER_PINKY,
      FINGER_PINKY_INVERT,
      CALIBRATION_CURL,
      PIN_FINGER_PINKY_SPLAY,
      FINGER_PINKY_SPLAY_INVERT,
      CALIBRATION_SPLAY
    ),
#elif FINGER_PINKY_ENABLED
    .pinky = FINGER_CLASS(IEncodedInput::Type::PINKY, PIN_FINGER_PINKY, FINGER_PINKY_INVERT, CALIBRATION_CURL),
#endif
};

#if BUTTON_CALIBRATE_ENABLED
std::optional<StringEncodedMemoizedSensor<bool>> calibrateButton =
  BUTTON_CLASS(IEncodedInput::Type::CALIBRATE, PIN_BUTTON_CALIBRATE, BUTTON_CALIBRATE_INVERT);
#else
std::optional<StringEncodedMemoizedSensor<bool>> calibrateButton = std::nullopt;
#endif

std::vector<StringEncodedMemoizedSensor<bool>*> buttons = std::vector<StringEncodedMemoizedSensor<bool>*>
{
    // https://github.com/llvm/llvm-project/issues/63776
    // clang-format off
#if BUTTON_A_ENABLED
    new BUTTON_CLASS(IEncodedInput::Type::A_BTN, PIN_BUTTON_A, BUTTON_A_INVERT),
#endif

#if BUTTON_B_ENABLED
    new BUTTON_CLASS(IEncodedInput::Type::B_BTN, PIN_BUTTON_B, BUTTON_B_INVERT),
#endif

#if BUTTON_JOYSTICK_ENABLED
    new BUTTON_CLASS(IEncodedInput::Type::JOY_BTN, PIN_BUTTON_JOYSTICK, BUTTON_JOYSTICK_INVERT),
#endif

#if BUTTON_MENU_ENABLED
    new BUTTON_CLASS(IEncodedInput::Type::MENU, PIN_BUTTON_MENU, BUTTON_MENU_INVERT),
#endif

#if GESTURE_TRIGGER_ENABLED && FINGER_INDEX_ENABLED
    new GESTURE_CLASS(
      IEncodedInput::Type::TRIGGER,
      new TriggerGesture(handSensors.index.value(), GESTURE_TRIGGER_THRESHOLD)
    ),
#elif BUTTON_TRIGGER_ENABLED
    new BUTTON_CLASS(IEncodedInput::Type::TRIGGER, PIN_BUTTON_TRIGGER, BUTTON_TRIGGER_INVERT),
#endif

#if GESTURE_GRAB_ENABLED && FINGER_INDEX_ENABLED && FINGER_MIDDLE_ENABLED && FINGER_RING_ENABLED && FINGER_PINKY_ENABLED
    new GESTURE_CLASS(
      IEncodedInput::Type::GRAB,
      new GrabGesture(
        handSensors.index.value(),
        handSensors.middle.value(),
        handSensors.ring.value(),
        handSensors.pinky.value(),
        GESTURE_GRAB_THRESHOLD
      )
    ),
#elif BUTTON_GRAB_ENABLED
    new BUTTON_CLASS(IEncodedInput::Type::GRAB, PIN_BUTTON_GRAB, BUTTON_GRAB_INVERT),
#endif

#if GESTURE_PINCH_ENABLED && FINGER_THUMB_ENABLED && FINGER_INDEX_ENABLED
    new GESTURE_CLASS(
      IEncodedInput::Type::PINCH,
      new PinchGesture(handSensors.thumb.value(), handSensors.index.value(), GESTURE_PINCH_THRESHOLD)
    ),
#elif BUTTON_PINCH_ENABLED
    new BUTTON_CLASS(IEncodedInput::Type::PINCH, PIN_BUTTON_PINCH, BUTTON_PINCH_INVERT),
#endif
    // clang-format on
};

std::vector<StringEncodedMemoizedSensor<uint16_t>*> joysticks = {
#if JOYSTICK_ENABLED
    new JOYSTICK_CLASS(IEncodedInput::Type::JOY_X, PIN_JOYSTICK_X, JOYSTICK_X_INVERT, JOYSTICK_DEADZONE),
    new JOYSTICK_CLASS(IEncodedInput::Type::JOY_Y, PIN_JOYSTICK_Y, JOYSTICK_Y_INVERT, JOYSTICK_DEADZONE),
#endif
    // todo: get joystick movement from Thumb finger movement?
};

std::vector<IStringEncodedMemoizedSensor*> otherSensors = std::vector<IStringEncodedMemoizedSensor*>();

OpenGlovesTrackingTaskConfig config =
  OpenGlovesTrackingTaskConfig(UPDATE_RATE, CALIBRATION_DURATION, CALIBRATION_ALWAYS_CALIBRATE);
OpenGlovesTrackingTask* trackingTask;

#if FFB_ENABLED
HandActuators handActuators = {
#if FFB_THUMB_ENABLED
    .thumb = new SenseShift::Arduino::Output::ServoActuator(PIN_FFB_THUMB),
#endif

#if FFB_INDEX_ENABLED
    .index = new SenseShift::Arduino::Output::ServoActuator(PIN_FFB_INDEX),
#endif

#if FFB_MIDDLE_ENABLED
    .middle = new SenseShift::Arduino::Output::ServoActuator(PIN_FFB_MIDDLE),
#endif

#if FFB_RING_ENABLED
    .ring = new SenseShift::Arduino::Output::ServoActuator(PIN_FFB_RING),
#endif

#if FFB_PINKY_ENABLED
    .pinky = new SenseShift::Arduino::Output::ServoActuator(PIN_FFB_PINKY),
#endif
};
OpenGlovesForceFeedbackTask* ffbTask;
#endif

void setupMode()
{
#if OPENGLOVES_COMMUNCATION == OPENGLOVES_COMM_SERIAL
    auto* communication = new SerialCommunication(SERIAL_PORT, SERIAL_BAUDRATE);
#elif OPENGLOVES_COMMUNCATION == OPENGLOVES_COMM_BTSERIAL
#ifdef BTSERIAL_NAME
    std::string name = BTSERIAL_NAME;
#else
    char suffix[4];
    sprintf(suffix, "%04X", (uint16_t) (ESP.getEfuseMac() >> 32));
    log_i("Generated Bluetooth suffix: %s", suffix);
    std::string name = BTSERIAL_PREFIX + std::string(suffix);
#endif
    BluetoothSerial* bt_serial = new BluetoothSerial();
    auto* communication = new BTSerialCommunication(*bt_serial, name);
#endif

    trackingTask = new OpenGlovesTrackingTask(
      config,
      *communication,
      handSensors,
      buttons,
      joysticks,
      otherSensors,
      calibrateButton,
      {
        .name = "OpenGlovesSensorTask",
        .stackDepth = 8192,
        .priority = OPENGLOVES_FINGERS_TASK_PRIORITY,
      }
    );

#if FFB_ENABLED
    ffbTask = new OpenGlovesForceFeedbackTask(
      *communication,
      handActuators,
      UPDATE_RATE,
      {
        .name = "OpenGlovesForceFeedbackTask",
        .stackDepth = 8192,
        .priority = OPENGLOVES_FINGERS_TASK_PRIORITY,
      }
    );
    ffbTask->begin();
#endif

    trackingTask->begin();
}

void loopMode()
{
    vTaskDelete(NULL);
}
