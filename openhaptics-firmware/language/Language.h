#ifndef LANGUAGE_H
#define LANGUAGE_H

class Language 
{
  public:
    virtual void decode(Stream* input);
};

#endif /* LANGUAGE_H */