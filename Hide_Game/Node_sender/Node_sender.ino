#include <RFM12B_arssi.h>

#define NODEID      99
#define NETWORKID   100
#define GATEWAYID   1
#define RFM_CS_PIN  10 // RFM12B Chip Select Pin
#define RFM_IRQ_PIN 2  // RFM12B IRQ Pin
#define FREQUENCY   RF12_433MHZ
#define SERIAL_BAUD 115200

//encryption is OPTIONAL
//to enable encryption you will need to:
// - provide a 16-byte encryption KEY (same on all nodes that talk encrypted)
// - to call .Encrypt(KEY) to start encrypting
// - to stop encrypting call .Encrypt(NULL)
uint8_t KEY[] = "YCBcXd5sQ4y976l7";

int TRANSMITPERIOD = 50;  //transmit a packet to gateway so often (in ms)
RFM12B radio;

typedef struct {
  uint8_t   code = 0x01;
  int8_t    rssi = RF12_ARSSI_RECV;
  String    greetings = "I am here";
} Payload;
Payload theData;

/*
 * setup for setting raadio module
 */
void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial.println(F("RFM12B Range Test Node"));
	Serial.println("RF Range Test");

  if ( radio.isPresent( RFM_CS_PIN, RFM_IRQ_PIN) )
  {
    Serial.println(F("RFM12B Detected OK!"));
  }
  else
  {
    Serial.println(F("RFM12B Detection FAIL! (is chip present?)"));
  }

  radio.Initialize(NODEID, FREQUENCY, NETWORKID, 0);
  radio.Encrypt(KEY);

  Serial.print(F("Transmitting at "));
  Serial.print(FREQUENCY==RF12_433MHZ ? 433 : FREQUENCY==RF12_868MHZ ? 868 : 915);
  Serial.println(F("Mhz..."));

  Serial.print("ID sender: ");
  Serial.print(NODEID);
  Serial.println();
  Serial.println(F("\nStarting to send data to gateway ...\n"));

  delay(2500);
}

/*
 * At loop this node only sends data every TRANSMITPERIOD ms
 */
void loop()
{ 
  // Send data to all gateway
  radio.Send(GATEWAYID, (const void*)(&theData), sizeof(theData), false);
  delay(TRANSMITPERIOD);
}
