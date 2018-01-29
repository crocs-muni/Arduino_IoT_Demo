/*
  Morse.cpp - Library for flashing Morse code.
  Created by Peter Mravec, January 14, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Morse.h"

Morse::Morse(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Morse::dot()
{
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
  delay(250);  
}

void Morse::dash()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}

void Morse::endLine()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
}

int Morse::getChars(char i)
{
  String chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  return chars.indexOf(i);
}

void Morse::codeString(String text)
{
  text.toUpperCase();
  int i = 0;
  for (i; i < text.length(); i++)
  {
    decide(text[i]);
  }
  Serial.println();
}

void Morse::decide(char c)
{
  switch (c){
    case 'A':
      A();
      break;
    case 'B':
      B();
      break;
    case 'C':
      C();
      break;
    case 'D':
      D();
      break;
    case 'E':
      E();
      break;
    case 'F':
      FF();
      break;
    case 'G':
      G();
      break;
    case 'H':
      H();
      break;
    case 'I':
      I();
      break;
    case 'J':
      J();
      break;
    case 'K':
      K();
      break;
    case 'L':
      L();
      break;
    case 'M':
      M();
      break;
    case 'N':
      N();
      break;
    case 'O':
      O();
      break;
    case 'P':
      P();
      break;
    case 'Q':
      Q();
      break;
    case 'R':
      R();
      break;
    case 'S':
      S();
      break;
    case 'T':
      T();
      break;
    case 'U':
      U();
      break;
    case 'V':
      V();
      break;
    case 'W':
      W();
      break;  
    case 'X':
      X();
      break;
    case 'Y':
      Y();
      break;
    case 'Z':
      Z();
      break;
    case ' ':
      gap();
      break;
    default:
      Serial.println("Do nothing");
  }
}

void Morse::A()
{
  Serial.println(". -");
  dot(); dash();
  Serial.println("get a");
}

void Morse::B()
{
  Serial.println("- . . .");
  dash(); dot(); dot(); dot();
  Serial.println("get b");
}

void Morse::C()
{
  Serial.println("- . - .");
  dash(); dot(); dash(); dot();
  Serial.println("get c");
}

void Morse::D()
{
  Serial.println("- . .");
  dash(); dot(); dot();
  Serial.println("get d");
}

void Morse::E()
{
  Serial.println(".");
  dot();
  Serial.println("get e");
}

void Morse::FF()
{
  Serial.println(". . - .");
  dot(); dot(); dash(); dot();
  Serial.println("get f");
}

void Morse::G()
{
  Serial.println("- - .");
  dash(); dash(); dot();
  Serial.println("get g");
}

void Morse::H()
{
  Serial.println(". . . .");
  dot(); dot(); dot(); dot();
  Serial.println("get h");
}

void Morse::I()
{
  Serial.println(". .");
  dot(); dot();
  Serial.println("get i");
}

void Morse::J()
{
  Serial.println(". - - -");
  dot(); dash(); dash(); dash();
  Serial.println("get j");
}

void Morse::K()
{
  Serial.println("- . -");
  dash(); dot(); dash();
  Serial.println("get k");
}

void Morse::L()
{
  Serial.println(". - . .");
  dot(); dash(); dot(); dot();
  Serial.println("get l");
}

void Morse::M()
{
  Serial.println("- -");
  dash(); dash();
  Serial.println("get m");
}

void Morse::N()
{
  Serial.println("- .");
  dash(); dot();
  Serial.println("get n");
}

void Morse::O()
{
  Serial.println("- - -");
  dash(); dash(); dash();
  Serial.println("get o");
}

void Morse::P()
{
  Serial.println(". - - .");
  dot(); dash(); dash(); dot();
  Serial.println("get p");
}

void Morse::Q()
{
  Serial.println("- - . -");
  dash(); dash(); dot(); dash();
  Serial.println("get q");
}

void Morse::R()
{
  Serial.println(". - .");
  dot(); dash(); dot();
  Serial.println("get r");
}

void Morse::S()
{
  Serial.println(". . .");
  dot(); dot(); dot();
  Serial.println("get s");
}

void Morse::T()
{
  Serial.println("-");
  dash();
  Serial.println("get t");
}

void Morse::U()
{
  Serial.println(". . -");
  dot(); dot(); dash();
  Serial.println("get u");
}

void Morse::V()
{
  Serial.println(". . . -");
  dot(); dot(); dot(); dash();
  Serial.println("get v");
}

void Morse::W()
{
  Serial.println(". - -");
  dot(); dash(); dash();
  Serial.println("get w");
}

void Morse::X()
{
  Serial.println("- . . -");
  dash(); dot(); dot(); dash();
  Serial.println("get x");
}

void Morse::Y()
{
  Serial.println("- . - -");
  dash(); dot(); dash(); dash();
  Serial.println("get y");
}

void Morse::Z()
{
  Serial.println("- - . .");
  dash(); dash(); dot(); dot();
  Serial.println("get z");
}

void Morse::gap()
{
  Serial.println("gap");
}

