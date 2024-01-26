#include <og_constants.hpp>
#include <opengloves_task.hpp>
#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/input/sensor/binary.hpp>
#include <senseshift/arduino/output/actuator/servo.hpp>
#include <senseshift/calibration.hpp>
#include <senseshift/input/sensor.hpp>
#include <senseshift/input/sensor/joystick.hpp>
#include <senseshift/opengloves/autoconfig.hpp>
#include <senseshift/opengloves/transport/stream.hpp>
#include <senseshift/utility.hpp>
#include <sensor/og_finger.hpp>
#include <sensor/og_gesture.hpp>

using namespace OpenGloves;
using namespace SenseShift::OpenGloves;

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
    .thumb = new SenseShift::Arduino::Output::ActuatorServo(PIN_FFB_THUMB),
#endif

#if FFB_INDEX_ENABLED
    .index = new SenseShift::Arduino::Output::ActuatorServo(PIN_FFB_INDEX),
#endif

#if FFB_MIDDLE_ENABLED
    .middle = new SenseShift::Arduino::Output::ActuatorServo(PIN_FFB_MIDDLE),
#endif

#if FFB_RING_ENABLED
    .ring = new SenseShift::Arduino::Output::ActuatorServo(PIN_FFB_RING),
#endif

#if FFB_PINKY_ENABLED
    .pinky = new SenseShift::Arduino::Output::ActuatorServo(PIN_FFB_PINKY),
#endif
};
OpenGlovesForceFeedbackTask* ffbTask;
#endif

void setupMode()
{
    auto* communication = AutoConfig::setupTransport();

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
    trackingTask->begin();

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
}

void loopMode()
{
    vTaskDelete(NULL);
}
