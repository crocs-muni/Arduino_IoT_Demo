#include <Wire.h>
#include <RFM12B_arssi.h>

#define NODEID      99
#define NETWORKID   100
#define GATEWAYID   1
#define RFM_CS_PIN  10 // RFM12B Chip Select Pin
#define RFM_IRQ_PIN 2  // RFM12B IRQ Pin
#define FREQUENCY   RF12_433MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define SERIAL_BAUD 115200
#define ACK_TIME    30  // # of ms to wait for an ack

#define LED    9

int TRANSMITPERIOD = 1000;    //transmit a packet to gateway so often (in ms)
byte sendSize=0;
RFM12B radio;
long lastPeriod = -1;

typedef struct {
  uint8_t   command;// command identifier
  int8_t    rssi;   // RSSI
} Payload;
Payload theData;

volatile uint8_t  adc_irq_cnt;

/* ======================================================================
Function: Interrupt routine for ADC
Purpose : Fired when ADC interrupt occured (mainly end of convertion)
Input   :
Output  :
Comments: Used by readADCLowNoise
====================================================================== */
ISR(ADC_vect)
{
  // Increment ADC sample count
  // will check after wake up
  adc_irq_cnt++;
}

/* ======================================================================
Function: readADCLowNoise
Purpose : Read Analog Value with reducing noise for more accuracy
Input   : true return the average value, false return only the sum
Output  : average value read
Comments: hard coded to read 8 samples each time
          ADMUX Channel must have been set before this call
====================================================================== */
uint16_t readADCLowNoise(bool average)
{
  uint8_t low, high;
  uint16_t sum = 0;

  // Start 1st Conversion, but ignore it, can be hazardous
  ADCSRA |= _BV(ADSC);

  // as we will enter into deep sleep mode, flush serial to avoid
  // data loss or corrupted
  Serial.flush();

  // wait for first dummy conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Initialize ADC sample counter
  adc_irq_cnt = 0;

  // Want to have an interrupt when the conversion is done
  ADCSRA |= _BV( ADIE );

  // Loop thru samples
  do
  {
    // Enable Noise Reduction Sleep Mode
    set_sleep_mode( SLEEP_MODE_ADC );
    sleep_enable();

    // Wait until conversion is finished
    do
    {
      // The following line of code is only important on the second pass.  For the first pass it has no effect.
      // Ensure interrupts are enabled before sleeping
      sei();
      // Sleep (MUST be called immediately after sei)
      sleep_cpu();
      // Checking the conversion status has to be done with interrupts disabled to avoid a race condition
      // Disable interrupts so the while below is performed without interruption
      cli();
    }
    while (bit_is_set(ADCSRA,ADSC));

    // No more sleeping
    sleep_disable();
    // Enable interrupts
    sei();

    // read low first
    low  = ADCL;
    high = ADCH;

    // Sum the total
    sum += ((high << 8) | low);

  }
  // Hard coded to read 8 samples
  while (adc_irq_cnt<8);

  // No more interrupts needed for this
  ADCSRA &= ~ _BV( ADIE );

  // Return the average divided by 8 (8 samples) if asked
  return ( average ? sum >> 3 : sum );

}

/* ======================================================================
Function: setup
Purpose : Configuration of Arduino I/O and other stuff
Input   : -
Output  : -
Comments:
====================================================================== */
void setup()
{

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

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

  Serial.println(F("Quick command summary, type it any time"));

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
  uint8_t  col;
  //process any serial input
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    if (input >= 48 && input <= 57) //[0,9]
    {
      TRANSMITPERIOD = 100 * (input-48);
      if (TRANSMITPERIOD == 0) TRANSMITPERIOD = 1000;
      Serial.print(F("\nChanging delay to "));
      Serial.print(TRANSMITPERIOD);
      Serial.println(F("ms\n"));
    }
  }

  //check for any received packets
  if (radio.ReceiveComplete())
  {
    if (radio.CRCPass())
    {
      int8_t rssi, k;

      // Light on the led
      digitalWrite(LED,HIGH);

      Serial.print('[');Serial.print(radio.GetSender(), DEC);Serial.print("] ");

      // len should be 1 (RSSI byte)
      if (*radio.DataLen != 1)
        Serial.print(F("Invalid RSSI payload received, not matching RSSI ACK Format!"));
      else
        rssi = radio.Data[0]; //radio.DATA actually contains RSSI


      if (rssi == RF12_ARSSI_DISABLED )
        Serial.print(F("disabled on gateway")); // ARSSI was not enabled on Gateway sketch
      else if (rssi == RF12_ARSSI_BAD_IDLE )
        Serial.print(F("has bad idle settings")); // Vidle for ARSSI has incorrect value on Gateway sketch
      else if (rssi == RF12_ARSSI_RECV )
        Serial.print(F("gateway RF reception in progress")); // can't get value another packet is in reception on GW
      else if (rssi == RF12_ARSSI_ABOVE_MAX )
        // Value above max limit, may be set up vidle is wrong on Gateway
        Serial.print(F("above maximum limit (measure and set vidle on gateway sketch"));
      else if (rssi == RF12_ARSSI_BELOW_MIN )
        // Value below min limit, may be set up vidle is wrong on Gateway
        Serial.print(F("below minimum limit (measure and set vidle on gateway sketch"));
      else if (rssi == RF12_ARSSI_NB_BYTES )
      {
        // We did not sent enough byte to gateway for accurate RSSI calculation
        Serial.print(F("not enough bytes ("));
        Serial.print(-(rssi-RF12_ARSSI_NB_BYTES));
        Serial.print(F(") sent to gateway to have accurate ARSSI"));
      }
      else
      {
        // all sounds good, display
        // display bargraph on serial
        Serial.print(F("["));

        for (k=RF12_ARSSI_MIN; k<=RF12_ARSSI_MAX; k++)
        {
          if (rssi>=k )
            Serial.print('=');
          else
            Serial.print(' ');
        }

        Serial.print(F("] "));
        Serial.print(rssi);
        Serial.print(F(" dB"));
      }


      Serial.println();

      // Light off the led
      digitalWrite(LED,LOW);

    }
  }

  int currPeriod = millis()/TRANSMITPERIOD;
  if (currPeriod != lastPeriod)
  {
    //fill in the payload with values
    theData.command = 0x01;
    theData.rssi = RF12_ARSSI_RECV;

    // Light on the led
    digitalWrite(LED,HIGH);

    // No ACK, but payload command permit to get back RSSI from master
    radio.Send(GATEWAYID, (const void*)(&theData), sizeof(theData), false);

    // Light off the led
    digitalWrite(LED,LOW);

    Serial.println();
    lastPeriod=currPeriod;
  }
}
