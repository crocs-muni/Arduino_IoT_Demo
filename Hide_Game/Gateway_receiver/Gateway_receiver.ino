#include <RFM12B_arssi.h>

#define NODEID      1
#define NETWORKID   100
#define FREQUENCY   RF12_433MHZ
#define RFM_CS_PIN  10 // RFM12B Chip Select Pin
#define RFM_IRQ_PIN 2  // RFM12B IRQ Pin
#define RSSI_PIN    0  // Analog entry where is connected ARSSI signal
#define SERIAL_BAUD 115200

//encryption is OPTIONAL
//to enable encryption you will need to:
// - provide a 16-byte encryption KEY (same on all nodes that talk encrypted)
// - to call .Encrypt(KEY) to start encrypting
// - to stop encrypting call .Encrypt(NULL)
uint8_t KEY[] = "YCBcXd5sQ4y976l7";

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 1000; // interval at which to blink (milliseconds)

int redPin = 6;
int greenPin = 15;
int bluePin = 17;

RFM12B radio;
byte ackCount=0;

typedef struct {
  uint8_t   code;
  int8_t    rssi;
  String    greetings;
} Payload;
Payload theData;

uint16_t vcc=0;
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
Function: readVcc
Purpose : Read and Calculate V powered, the Voltage on Arduino VCC pin
Input   : -
Output  : value readed in mV
Comments: ADC Channel input is modified
====================================================================== */
uint16_t readVcc()
{
  uint16_t value;

  // Indicate RFM12B IRQ we're doing conversion
  // so it should not doing any RSSI acquisition in the interval
  // we're using the ADC
  radio.noRSSI(true);

  // Read 1.1V reference against AVcc
  // REFS1 REFS0          --> 0 1, AVcc external ref. -Selects AVcc external reference
  // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
  ADMUX = (0<<REFS1) | (1<<REFS0) | (0<<ADLAR) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (0<<MUX0);

  // Take care, changing reference from VCC to 1.1V bandgap can take some time, this is due
  // to the fact that the capacitor on aref pin need to discharge or to charge
  delay(10);

  // read value
  value = readADCLowNoise(true);

  // we done with ADC
  radio.noRSSI(false);

  // Vcc reference in millivolts
  // can be adjusted 1100L if 1V1 reference but has tolerance of 10% so you can measure it
  // and change it there, or better use it as parameter
  return ( (( 1023L * 1100L) / value) );
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
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(SERIAL_BAUD);
  delay(10);
  Serial.println(F("RFM12B Range Test Gateway"));

  if ( radio.isPresent( RFM_CS_PIN, RFM_IRQ_PIN) )
    Serial.println(F("RFM12B Detected OK!"));
  else
    Serial.println(F("RFM12B Detection FAIL! (is chip present?)"));

  radio.SetRSSI( RSSI_PIN, 350 );

  radio.Initialize(NODEID, FREQUENCY, NETWORKID);
  radio.Encrypt(KEY);

  Serial.print(F("Transmitting at "));
  Serial.print(FREQUENCY==RF12_433MHZ ? 433 : FREQUENCY==RF12_868MHZ ? 868 : 915);
  Serial.println(F("Mhz..."));

  // If RSSI is activated for this board
  // display relative information
  if ( radio.getRSSIIdle())
  {
    Serial.print(F("ARSSI Enabled\nConnect ARSSI signal on Analog pin "));
    Serial.print(radio.getRSSIAnalogPin());
    Serial.print(F(" of this board\nARSSI idle voltage is set to "));
    Serial.print(radio.getRSSIIdle());
    Serial.println(F(" mV"));
  }
  else
  {
    Serial.println(F("ARSSI Disabled for this board"));
  }


  // read vcc value
  vcc=readVcc();
  Serial.print(F("Vcc = "));
  Serial.print(vcc);
  Serial.println(F(" mV"));

  Serial.println(F("\nWaiting for receiving data from node ...\n"));

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
  if (radio.ReceiveComplete())
  {
    if (radio.CRCPass())
    {
      // Get RSSI of received packet
      // Set parameter to VCC voltage of arduino
      byte theNodeID = radio.GetSender();

      int8_t rssi = radio.ReadARSSI(vcc);
      int8_t k;

      Serial.print(F("["));
      Serial.print(theNodeID, DEC);
      Serial.print("] ");

      if (*radio.DataLen==sizeof(Payload))
      {
        // when I receive signal, this I need to determine time between receiving signal
        previousMillis = millis();

        theData = *(Payload*)radio.Data;

        // Ok is it a RSSI request packet ?
        if (theData.rssi==RF12_ARSSI_RECV && theData.code==0x01)
        {
          Serial.flush();

          // Send RSSI back
          radio.Send(theNodeID, &rssi, 1, false);


          if (rssi == RF12_ARSSI_DISABLED )
            Serial.print(F("disabled")); // ARSSI was not enabled on sketch
          else if (rssi == RF12_ARSSI_BAD_IDLE )
            Serial.print(F("has bad idle settings")); // Vidle for ARSSI has incorrect value
          else if (rssi == RF12_ARSSI_RECV )
            Serial.print(F("gateway RF reception in progress")); // can't get value another packet is in reception
          else if (rssi == RF12_ARSSI_ABOVE_MAX )
            // Value above max limit, may be set up vidle is wrong
            Serial.print(F("above maximum limit (measure and set vidle on gateway sketch"));
          else if (rssi == RF12_ARSSI_BELOW_MIN )
            // Value below min limit, may be set up vidle is wrong
            Serial.print(F("below minimum limit (measure and set vidle on gateway sketch"));
          else if (rssi == RF12_ARSSI_NB_BYTES )
          {
            // We did not sent enough byte received to for accurate RSSI calculation
            Serial.print(F("not enough bytes ("));
            Serial.print(-(rssi-RF12_ARSSI_NB_BYTES));
            Serial.print(F(") sent to gateway to have accurate ARSSI"));
          }
          else
          {
            // all sounds good, display
            // display bargraph
            Serial.print(F("["));

            for (k=RF12_ARSSI_MIN; k<=RF12_ARSSI_MAX; k++)
              Serial.print(rssi>=k ? '=' : ' ');

            Serial.print(F("] "));

            Serial.print(rssi);
            Serial.print(F(" dB"));
            if (rssi < -89){
              setColor(0, 255, 0);
              Serial.println("Far green color");
            }
            else
            {
              if (rssi < -80)
              {
                setColor(245, 245, 0);
                Serial.println("Near orange");
              }
              else
              {
                setColor(255, 0, 0);
                Serial.println("Got red");
              }
            }
          }
        }
      }
      else
      {
        Serial.print(F("Invalid payload received, not matching Payload struct!"));
      }

      Serial.println();
    }
  }
  else
  {
    unsigned long currentMillis = millis();

    // without delay function
    if (currentMillis - previousMillis >= interval) {    // switch off RGB diod
      setColor(0, 0, 0);
      previousMillis = 0;
    }
  }
}

/*
 * Method controls color of RGB diode
 */
void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
