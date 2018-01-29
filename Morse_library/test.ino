#include "Morse.h"

Morse morse(9);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("*****     START      *****");
  morse.codeString("soS");
  Serial.println("*****     END      *****");
}
