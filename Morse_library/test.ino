#include "Morse.h"

Morse morse(9);

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
}

void loop() {
  Serial.println("*****     START      *****");
  morse.codeString  ("SoSandRUN");
  Serial.println("*****     END      *****");
}