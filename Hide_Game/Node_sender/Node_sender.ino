#include <Wire.h>
#include <RFM12B_arssi.h>

#define NODEID      99
#define NETWORKID   100
#define GATEWAYID   1
#define RFM_CS_PIN  10 // RFM12B Chip Select Pin
#define RFM_IRQ_PIN 2  // RFM12B IRQ Pin
#define FREQUENCY   RF12_433MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define SERIAL_BAUD 115200

int TRANSMITPERIOD = 100;    //transmit a packet to gateway so often (in ms)
byte sendSize=0;
RFM12B radio;

typedef struct {
  uint8_t   command;// command identifier
  int8_t    rssi;   // RSSI
} Payload;
Payload theData;


/* ======================================================================
Function: setup
Purpose : Configuration of Arduino I/O and other stuff
Input   : -
Output  : -
Comments:
====================================================================== */
void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("RFM12B Range Test Node"));
	Serial.println("RF Range Test");

  // Try to detect and Init On moteino RF12 device SS is D10 and IRQ D2 (default)
  // So the parameters are optional
  // if radio.isPresent()
  if ( radio.isPresent( RFM_CS_PIN, RFM_IRQ_PIN) )
  {
    Serial.println(F("RFM12B Detected OK!"));
  }
  else
  {
    Serial.println(F("RFM12B Detection FAIL! (is chip present?)"));
  }


  radio.Initialize(NODEID, FREQUENCY, NETWORKID, 0);

  Serial.print(F("Transmitting at "));
  Serial.print(FREQUENCY==RF12_433MHZ ? 433 : FREQUENCY==RF12_868MHZ ? 868 : 915);
  Serial.println(F("Mhz..."));

  Serial.print(F("ARSSI "));


  // If RSSI is activated for this board
  // display relative information
  if ( radio.getRSSIIdle())
  {
    Serial.print(F("Enabled\nConnect ARSSI signal on Analog pin "));
    Serial.print(radio.getRSSIAnalogPin());
    Serial.print(F(" of this board\nARSSI idle voltage is set to "));
    Serial.print(radio.getRSSIIdle());
    Serial.println(F(" mV"));
  }
  else
  {
    Serial.println(F("Disabled for this board"));
  }

  Serial.println(F("\nStarting to send data to gateway ...\n"));

  delay(2500);
}

/* ======================================================================
Function: loop
Purpose : really need explanations ???
Input   : -
Output  : -
Comments:
====================================================================== */
void loop()
{ 
  //fill in the payload with values
  theData.command = 0x01;
  theData.rssi = RF12_ARSSI_RECV;

  // Send date to all gateway
  radio.Send(GATEWAYID, (const void*)(&theData), sizeof(theData), false);
  delay(TRANSMITPERIOD);
}
