// Simple RFM12B sender program, with ACK and optional encryption
// It initializes the RFM12B radio with optional encryption and passes through any valid messages to the serial port
// felix@lowpowerlab.com

#include "RFM12B.h"
#include "DHT.h"
#include <avr/sleep.h>

#define DHTPIN 6     // what digital pin we're connected to
#define LED 9

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// You will need to initialize the radio by telling it what ID it has and what network it's on
// The NodeID takes values from 1-127, 0 is reserved for sending broadcast messages (send to all nodes)
// The Network ID takes values from 0-255
// By default the SPI-SS line used is D10 on Atmega328. You can change it by calling .SetCS(pin) where pin can be {8,9,10}
#define NODEID        2  //network ID used for this unit
#define NETWORKID    99  //the network ID we are on
#define GATEWAYID     1  //the node ID we're sending to
#define ACK_TIME     50  // # of ms to wait for an ack
#define SERIAL_BAUD  9600    //115200

typedef struct {
  float      temperature;    // measured tempreture in Celsius
  float       humidity;   // measured humidity
} Payload;
Payload theData;

//encryption is OPTIONAL
//to enable encryption you will need to:
// - provide a 16-byte encryption KEY (same on all nodes that talk encrypted)
// - to call .Encrypt(KEY) to start encrypting
// - to stop encrypting call .Encrypt(NULL)
uint8_t KEY[] = "ABCDABCDABCDABCD";

int interPacketDelay = 1000; //wait this many ms between sending packets
char input = 0;

// Need an instance of the Radio Module
RFM12B radio;
byte sendSize=0;
bool requestACK=false;

static Payload getDataFromSenzor(DHT dht){
  Payload data;
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  data.humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  data.temperature = dht.readTemperature();
  // It is also possible read temperature as Fahrenheit (isFahrenheit = true)
  // for exaple: float fahrenhaitTemperature = dht.readTemperature(true);
  return data;
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  radio.Encrypt(KEY);
  radio.Sleep(); //sleep right away to save power
  Serial.println("Begin measure!");
  Serial.println("Transmitting...\n\n");
  dht.begin();
  pinMode(LED, OUTPUT);
}

void loop()
{
  // Wait a few seconds between measurements.
  delay(2000);

  theData = getDataFromSenzor(dht);

  // Check if any reads failed and exit early (to try again).
  if (isnan(theData.humidity) || isnan(theData.temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(theData.humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(theData.temperature);
  Serial.print(" *C ");

  Serial.print("Sending[");
  Serial.print(sendSize+1);
  Serial.print("]:");

  requestACK = !(sendSize % 3); //request ACK every 3rd xmission

  radio.Wakeup();
  makeBlick();
  radio.Send(GATEWAYID, (const void*)(&theData), sizeof(theData), requestACK);
  if (requestACK)
  {
    Serial.print(" - waiting for ACK...");
    if (waitForAck()) Serial.print("ok!");
    else Serial.print("nothing...");
  }
  radio.Sleep();

  sendSize = (sendSize + 1) % 88;
  Serial.println();
  delay(interPacketDelay);
}

// make a signal, we can determine that station is still working
static void makeBlick(){
  digitalWrite(LED, LOW);
  delay(50);
  digitalWrite(LED, HIGH);
}

// wait a few milliseconds for proper ACK, return true if received
static bool waitForAck() {
  long now = millis();
  while (millis() - now <= ACK_TIME)
    if (radio.ACKReceived(GATEWAYID))
      return true;
  return false;
}
