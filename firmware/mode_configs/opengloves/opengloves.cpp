#include "senseshift/body/hands/input/gesture.hpp"
#include "senseshift/body/hands/input/total_curl.hpp"
#include <senseshift/arduino/input/sensor/analog.hpp>
#include <senseshift/arduino/input/sensor/digital.hpp>
#include <senseshift/body/hands/input/gesture.hpp>
#include <senseshift/input/calibration.hpp>
#include <senseshift/input/filter.hpp>
#include <senseshift/input/sensor.hpp>
#include <senseshift/opengloves/autoconfig.hpp>
#include <senseshift/utility.hpp>

using namespace ::SenseShift::Input;
using namespace ::SenseShift::Arduino::Input;
using namespace ::SenseShift::Body::Hands::Input;
using namespace ::SenseShift::OpenGloves;

InputSensors input_sensors;

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
}

void loopMode()
{
    vTaskDelete(NULL);
}
