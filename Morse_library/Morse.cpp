/*
  Morse.cpp - Library for flashing Morse code.
  Created by Peter Mravec, January 14, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Morse.h"

/*
 * ALPHABET contains morse code
 * . is dot
 * - id dash
 */
String ALPHABET[26] = {
//morse code   alphabet char
    ".-",      //  A
    "-...",    //  B
    "-.-.",    //  C
    "-..",     //  D
    ".",       //  E
    "..-.",    //  F
    "--.",     //  G
    "....",    //  H
    "..",      //  I
    ".---",    //  J
    "-.-",     //  K
    ".-..",    //  L
    "--",      //  M
    "-.",      //  N
    "---",     //  O
    ".--.",    //  P
    "--.-",    //  Q
    ".-.",     //  R
    "...",     //  S
    "-",       //  T
    "..-",     //  U
    "...-",    //  V
    ".--",     //  W
    "-..-",    //  X
    "-.--",    //  Y
    "--.."     //  Z
  };

/*
 * initial setup for pinMode(pin) and
 * store value of pin as private _pin
 */
void Morse::initialize(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

/*
 * method for visualing dot with blink LED on _pin
 */
void Morse::dot()
{
  digitalWrite(_pin, LOW);
  delay(250);
  digitalWrite(_pin, HIGH);
  delay(250);
}

/*
 * method for visualing dash with blink LED on _pin
 */
void Morse::dash()
{
  digitalWrite(_pin, LOW);
  delay(750);
  digitalWrite(_pin, HIGH);
  delay(250);
}

/*
 * delay between 2 characters is 3 * unit (250)
 * => 750 ms
 */
void Morse::delayChar()
{
  Serial.println("delaying ");
  digitalWrite(_pin, HIGH);
  delay(3 * 250);
}

/*
 * delay between words is 7 * unit (250)
 * but it is word word so it has to be - 3 * unit (250)
 * => there is only 4 * unit (250)
 * => 1000 ms
 */
void Morse::delayWord()
{
  Serial.println("delaying ");
  digitalWrite(_pin, HIGH);
  delay(4 * 250);
}

/*
 * method for encode text to visualing in morse code
 * Example codeString("I AM PETER")
 * Allowed only this characters:
 * [ABCDEFGHIJKLMNOPQRSTUVWXYZ ]
 */
void Morse::codeString(String text)
{
  text.toUpperCase();
  int i = 0;
  for (i; i < text.length(); i++)
  {
    Serial.println("call to encode character  ");
    Serial.print(text[i]);
    encodeChar(text[i]);
  }
}

/*
 * method calls dot() and dash()
 * at the end calls delayChar()
 */
void Morse::do_signal(String encode_string){
  int i;
  Serial.println(encode_string);
  for (i = 0; i < encode_string.length(); i++){
    if (encode_string[i] == '.'){
      Serial.println("dot");
      dot();
    }
    if (encode_string[i] == '-'){
      Serial.println("dash");
      dash();
    }
  };
  delayChar();
}

/*
 * method calls other methods according to 
 * value of character
 * if character is not in authorized alphabet => do nothing
 */
void Morse::encodeChar(char character)
{
  if (character == ' '){
    delayWord();
  }
  else {
    int ord = character - 'A';
    if (ord >= 0 and ord <= 25){
      Serial.println("ord ");
      Serial.print(ord);
      Serial.println(ALPHABET[ord]);
      do_signal(ALPHABET[ord]);
    }
  }
}
