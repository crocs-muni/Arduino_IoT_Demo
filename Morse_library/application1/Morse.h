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
    Morse(int pin);
    void dot();
    void dash();
    void endLine();
    void codeString(String text);
    void encodeChar(char character);
    void do_gap();
    void do_signal (String encode_string  );
  private:
    int _pin;
};

#endif
