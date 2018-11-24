/*
 * simple example of how to use Morse library
 */

#include "Morse.h"

Morse morse;

void setup() {
  Serial.begin(9600);
  morse.initialize(9);
}

void loop() {
  Serial.println("*****     START      *****");
  // it is possible call with lowercase or uppercase characters
  // " " is allowed only between 2 words
  morse.codeString  ("SOs sos");
  Serial.println("*****     END        *****");
}
