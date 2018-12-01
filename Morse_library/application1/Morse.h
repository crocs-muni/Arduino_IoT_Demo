/*
  Morse.h - Library for flashing Morse code.
  Created by Peter Mravec, January 14, 2018.
  Released into the public domain.
*/
#ifndef Morse_h
#define Morse_h

#include "Arduino.h"

class Morse
{
  public:
    void initialize(int pin);
    void dot();
    void dash();
    void delayWord();
    void delayChar();
    void codeString(String text);
    void encodeChar(char character);
    void do_signal (String encode_string  );
  private:
    int _pin;
};

#endif
