#ifndef LANGUAGE_BHAPTICS_PLAYER_H
#define LANGUAGE_BHAPTICS_PLAYER_H

#include "Language.h"

class LanguageBHapticsPlayer : public Language
{
  public:
    LanguageBHapticsPlayer(void);
    void decode(Stream* input);
};

#endif /* LANGUAGE_BHAPTICS_PLAYER_H */
