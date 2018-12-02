/*
 * Example of how it is possible to create parking senzor using IOT
 * I used NewPing.h library to measure distance
 */

#include <NewPing.h> //https://bitbucket.org/teckel12/arduino-new-ping/downloads/

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 250 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define SERIAL_BAUD 115200

int Buzzer = 13;
int val=0;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() 
{
  Serial.begin(SERIAL_BAUD);
  pinMode(Buzzer,OUTPUT); //Setup Buzzer pin as output pin
}

void loop() 
{
  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  val=sonar.ping_cm();
  Serial.print("Distance is ");
  Serial.print(val);
  Serial.println(" cm.");
  
  if(val<=10&&val>0)
  {
    digitalWrite(Buzzer, HIGH);   // turn the Buzzer on (HIGH is the voltage level)
    //ziaden zvuk
  }
  else if(val<=25&&val>10)
  {
    digitalWrite(Buzzer, HIGH);   // turn the Buzzer on (HIGH is the voltage level)
    delay(50);               // wait for a second
    digitalWrite(Buzzer, LOW);    // turn the Buzzer off by making the voltage LOW
    delay(50); 
  }
  else if(val<=50&&val>25)
  {
    digitalWrite(Buzzer, HIGH);   // turn the Buzzer on (HIGH is the voltage level)
    delay(75);               // wait for a second
    digitalWrite(Buzzer, LOW);    // turn the Buzzer off by making the voltage LOW
    delay(75);               // wait for a second
  }
    else if(val<=75&&val>50)
  {
    digitalWrite(Buzzer, HIGH);   // turn the Buzzer on (HIGH is the voltage level)
    delay(100);               // wait for a second
    digitalWrite(Buzzer, LOW);    // turn the Buzzer off by making the voltage LOW
    delay(100);  
  }
    else if(val<=100&&val>75)
  {
    digitalWrite(Buzzer, HIGH);   // turn the Buzzer on (HIGH is the voltage level)
    delay(200);               // wait for a second
    digitalWrite(Buzzer, LOW);    // turn the Buzzer off by making the voltage LOW
    delay(200);   
  }  
      else if(val<MAX_DISTANCE&&val!=0)
  {
            // wait for a second
    digitalWrite(Buzzer, LOW);    // turn the Buzzer off by making the voltage LOW
    Serial.println("*** More than 100 cm");
    Serial.print("Val is ");
    Serial.print(val);
    Serial.println();

  }  
}

