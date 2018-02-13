/*
  Morse.cpp - Library for flashing Morse code.
  Created by Peter Mravec, January 14, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Morse.h"

String ALPHABET[26] = {
    ".-",      //A
    "-...",    //B
    "-.-.",    //C
    "-..",      //D
    ".",          //E
    "..-.",    //F
    "--.",      //G
    "....",    //H
    "..",        //I
    ".---",    //J
    "-.-",      //K
    ".-..",    //L
    "--",        //M
    "-.",        //N
    "---",      //O
    ".--.",    //P
    "--.-",     //Q
    ".-.",      //R
    "...",      //S
    "-",          //T
    "..-",      //U
    "...-",    //V
    ".--",      //W
    "-..-",    //X
    "-.--",     //Y
    "--.."     //Z
  };

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

void Morse::codeString(String text)
{
  text.toUpperCase();
  //Serial.println(ALPHABET[10].charAt(0));
  int i = 0;
  for (i; i < text.length(); i++)
  {
    Serial.println("call to encode ord and character");
    Serial.println(text[i]);
    encodeChar(text[i]);
  }
}

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
  endLine();
}

void Morse::do_gap(){
  Serial.println("gap");
}

void Morse::encodeChar(char character)
{
  Serial.println("call encodeCHAR with ");
  Serial.println(character);
  if (character == ' '){
    do_gap();
    return;
  }
  int ord = character - 'A';
  if (ord >= 0 and ord <= 25){
    Serial.println("ord and char");
    Serial.println(ord);
    Serial.println(ALPHABET[ord]);
    Serial.println(_pin);
    Serial.println(ALPHABET[0]);
    do_signal(ALPHABET[ord]);
  }
}