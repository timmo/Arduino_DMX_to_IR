
// This code mappes dmx values to IR codes.

#include <lib_dmx.h>  // http://www.deskontrol.net/blog/arduino-four-universes-dmx-512-library/
#include <IRremote.h> // https://github.com/shirriff/Arduino-IRremote

#define    DMX512     (0)    // (250 kbaud - 2 to 512 channels) Standard USITT DMX-512

// PINS
const int SEND_PIN = 3;         // Put IR LED with 100 Ohm resistor to ground here
const int MAX485_IO_CTRL = 6;   // Arduino output pin for MAX485 input/output control (connect to MAX485 pins 2-3) 
const int IR_SEND_LED_PIN = 13; // Builtin LED to signal IR send

const int DMX_START_ADDRESS = 5; // DMX start address
const int DMX_CHANNEL_COUNT = 2; // Number of rx channels

// IR Commands
const unsigned long IR_COMMAND_LENGTH_IN_BIT = 32; // NEC IR remote
const unsigned long IR_ON = 0xF7C03F;
const unsigned long IR_OFF = 0xF740BF;
const unsigned long IR_BRIGHTER = 0xF700FF;
const unsigned long IR_DARKER = 0xF7807F;
const unsigned long IR_COLOR_CODES [] = {
  0xF7A05F, // Green
  0xF7906F,
  0xF7B04F, // Light blue
  0xF78877,
  0xF7A857,

  0xF7609F, // Blue
  0xF750AF,
  0xF7708F,
  0xF748B7,
  0xF76897, // purple 

  0xF720DF, // RED
  0xF710EF,
  0xF730CF,
  0xF708F7,
  0xF728D7, // Yellow

  0xF7E01F // White
};
const int IR_COLOR_CODES_LENGTH = sizeof(IR_COLOR_CODES)/sizeof(IR_COLOR_CODES[0]);

// Variables
unsigned long lastOnOffCode = IR_OFF;
int lastColorIndex = 0;
IRsend irsend;

void setup() 
{
  // setup ir send led
  pinMode(IR_SEND_LED_PIN, OUTPUT);
  digitalWrite(IR_SEND_LED_PIN, LOW);

  // setup dmx library
  ArduinoDmx0.set_control_pin(MAX485_IO_CTRL);    
  ArduinoDmx0.set_rx_address(DMX_START_ADDRESS);      
  ArduinoDmx0.set_rx_channels(DMX_CHANNEL_COUNT);     
  ArduinoDmx0.init_rx(DMX512);        // starts universe 0 as rx, NEW Parameter DMX mode
}  

void loop()
{
  int dmxValue0 = ArduinoDmx0.RxBuffer[0];
  int dmxValue1 = ArduinoDmx0.RxBuffer[1];

  unsigned long onOffCode = dmxValue0 < 128 ? IR_OFF : IR_ON;
  int colorIndex = dmxValue1 / IR_COLOR_CODES_LENGTH;

  if (onOffCode != lastOnOffCode) {
    digitalWrite(IR_SEND_LED_PIN, HIGH);
    irSendWithDelay(onOffCode);
    digitalWrite(IR_SEND_LED_PIN, LOW);
  }
  
  if (colorIndex != lastColorIndex) {
    digitalWrite(IR_SEND_LED_PIN, HIGH);
    irSendWithDelay(IR_COLOR_CODES[colorIndex]);
    digitalWrite(IR_SEND_LED_PIN, LOW);
  }
  
  lastOnOffCode = onOffCode;
  lastColorIndex = colorIndex;
}

void irSendWithDelay(unsigned long data) {
  for (int i = 0; i < 3; i++) {
    irsend.sendNEC(data, IR_COMMAND_LENGTH_IN_BIT);
    delay(40);
  }
}













