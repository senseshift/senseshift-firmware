#include "senseshift/body/hands/input/gesture.hpp"
#include "senseshift/body/hands/input/total_curl.hpp"
#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/input/sensor/digital.hpp>
#include <senseshift/arduino/output/servo.hpp>
#include <senseshift/body/hands/input/gesture.hpp>
#include <senseshift/input/calibration.hpp>
#include <senseshift/input/filter.hpp>
#include <senseshift/input/sensor.hpp>
#include <senseshift/opengloves/autoconfig.hpp>
#include <senseshift/opengloves/opengloves.hpp>
#include <senseshift/opengloves/opengloves_task.hpp>
#include <senseshift/utility.hpp>

using namespace ::SenseShift::Input;
using namespace ::SenseShift::Arduino::Input;
using namespace ::SenseShift::Arduino::Output;
using namespace ::SenseShift::Body::Hands::Input;
using namespace ::SenseShift::OpenGloves;

InputSensors input_sensors;

#ifdef FFB_ENABLED
OutputWriters output_writers;
#endif

void setupMode()
{
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
    auto* button_a = new BUTTON_CLASS(PIN_BUTTON_A, BUTTON_A_INVERT);
    input_sensors.button_a.press = button_a;
#endif
#if BUTTON_B_ENABLED
    auto* button_b = new BUTTON_CLASS(PIN_BUTTON_B, BUTTON_B_INVERT);
    input_sensors.button_b.press = button_b;
#endif
#if BUTTON_JOYSTICK_ENABLED
    auto* button_joystick = new BUTTON_CLASS(PIN_BUTTON_JOYSTICK, BUTTON_JOYSTICK_INVERT);
    input_sensors.joystick.press = button_joystick;
#endif
#if BUTTON_MENU_ENABLED
    auto* button_menu = new BUTTON_CLASS(PIN_BUTTON_MENU, BUTTON_MENU_INVERT);
#endif
#if BUTTON_CALIBRATE_ENABLED
    auto* button_calibrate = new BUTTON_CLASS(PIN_BUTTON_CALIBRATE, BUTTON_CALIBRATE_INVERT);
    input_sensors.button_calibrate.press = button_calibrate;
#endif

#if GESTURE_TRIGGER_ENABLED
    auto* trigger = new TriggerGesture(index_curl_sensor, GESTURE_TRIGGER_THRESHOLD);
    input_sensors.trigger.press = trigger;
#elif BUTTON_TRIGGER_ENABLED
    auto trigger = new BUTTON_CLASS(PIN_BUTTON_TRIGGER, BUTTON_TRIGGER_INVERT);
#endif

#if GESTURE_GRAB_ENABLED
    auto* grab = new GrabGesture(
      GrabGesture::Fingers{ .index = index_curl_sensor,
                            .middle = middle_curl_sensor,
                            .ring = ring_curl_sensor,
                            .pinky = pinky_curl_sensor },
      GESTURE_GRAB_THRESHOLD
    );
    input_sensors.grab.press = grab;
#elif BUTTON_GRAB_ENABLED
    auto* grab = new BUTTON_CLASS(PIN_BUTTON_GRAB, BUTTON_GRAB_INVERT);
#endif

#if GESTURE_PINCH_ENABLED
    auto* pinch = new PinchGesture(
      PinchGesture::Fingers{ .thumb = thumb_curl_sensor, .index = index_curl_sensor },
      GESTURE_PINCH_THRESHOLD
    );
    input_sensors.pinch.press = pinch;
#elif BUTTON_PINCH_ENABLED
    auto* pinch = new BUTTON_CLASS(PIN_BUTTON_PINCH, BUTTON_PINCH_INVERT);
#endif

    auto* communication = AutoConfig::setupTransport();
    auto* encoding = new og::AlphaEncoder();
    OpenGlovesTrackingComponent::Config tracking_config(CALIBRATION_DURATION, CALIBRATION_ALWAYS_CALIBRATE);
    auto* opengloves_tracking =
      new OpenGlovesTrackingComponent(tracking_config, input_sensors, communication, encoding);

    auto* opengloves_tracking_task = new ::SenseShift::FreeRTOS::ComponentUpdateTask<OpenGlovesTrackingComponent>(
      opengloves_tracking,
      1000 / UPDATE_RATE,
      {
        .name = "OG_TRACKING",
        .stackDepth = 8192,
        .priority = 1,
      }
    );
    opengloves_tracking_task->begin();

#if FFB_ENABLED

#if FFB_THUMB_ENABLED
    auto* thumb_ffb_output = new ServoOutput(PIN_FFB_THUMB);
    output_writers.ffb.thumb = thumb_ffb_output;
#endif

#if FFB_INDEX_ENABLED
    auto* index_ffb_output = new ServoOutput(PIN_FFB_INDEX);
    output_writers.ffb.index = index_ffb_output;
#endif

#if FFB_MIDDLE_ENABLED
    auto* middle_ffb_output = new ServoOutput(PIN_FFB_MIDDLE);
    output_writers.ffb.middle = middle_ffb_output;
#endif

#if FFB_RING_ENABLED
    auto* ring_ffb_output = new ServoOutput(PIN_FFB_RING);
    output_writers.ffb.ring = ring_ffb_output;
#endif

#if FFB_PINKY_ENABLED
    auto* pinky_ffb_output = new ServoOutput(PIN_FFB_PINKY);
    output_writers.ffb.pinky = pinky_ffb_output;
#endif

    auto* og_ffb = new OpenGlovesForceFeedbackComponent(output_writers, communication, encoding);

    auto* og_ffb_task = new ::SenseShift::FreeRTOS::ComponentUpdateTask<OpenGlovesForceFeedbackComponent>(
      og_ffb,
      1000 / UPDATE_RATE,
      {
        .name = "OG_FFB",
        .stackDepth = 8192,
        .priority = 1,
      }
    );
    og_ffb_task->begin();
#endif
}

void loopMode()
{
    vTaskDelete(NULL);
}
