#include "LanguageBHapticsPlayer.h"

#include <Arduino.h>
#include <ArduinoJson.h>

LanguageBHapticsPlayer::LanguageBHapticsPlayer() {}

void LanguageBHapticsPlayer::decode(Stream* input)
{
    DynamicJsonDocument obj(2048);

    DeserializationError err = deserializeJson(obj, *input);

    if (err != DeserializationError::Ok) 
    {
        return;
    }

    if (obj.containsKey("Submit")) {
        Serial.println("Submit");
    } else {
        Serial.println("NOT Submit");
    }
}