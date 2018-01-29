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
    int getChars(char i);
    void codeString(String text);
    void decide(char c);
    void A();
    void B();
    void C();
    void D();
    void E();
    void FF();
    void G();
    void H();
    void I();
    void J();
    void K();
    void L();
    void M();
    void N();
    void O();
    void P();
    void Q();
    void R();
    void S();
    void T();
    void U();
    void V();
    void W();
    void X();
    void Y();
    void Z();
    void gap();
  private:
    int _pin;
};

#endif
