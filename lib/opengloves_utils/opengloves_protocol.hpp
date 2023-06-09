#pragma once

namespace OpenGloves
{
  struct EncodedInput {
    enum Type : char {
      THUMB = 'A',
      INDEX = 'B',
      MIDDLE = 'C',
      RING = 'D',
      PINKY = 'E',
      JOY_X = 'F',
      JOY_Y = 'G',
      JOY_BTN = 'H',
      TRIGGER = 'I',
      A_BTN = 'J',
      B_BTN = 'K',
      GRAB = 'L',
      PINCH = 'M',
      MENU = 'N',
      CALIBRATE = 'O'
    };
  };
} // namespace OpenGloves
