#include <SPI.h>
#include <Arduino.h>
#include <MIDI.h>
#include <NeoPixelBus.h>
#include "driver/spi_slave.h"


volatile boolean received;
int pitch;
volatile byte pos;
volatile byte pos2;
volatile boolean process_it;
unsigned long lastUpdate;
unsigned long currentTime;
bool update;
bool leds [88];
bool updatedLeds [88];

const uint16_t PixelCount = 172; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 1;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
/* The product now has two screens, and the initialization code needs a small change in the new version. The LCD_MODULE_CMD_1 is used to define the
 * switch macro. */

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);
void handleNoteOn(byte channel, byte pitch, byte velocity) { 
  
  pitch -= 21;
  leds[pitch] = true;
  updatedLeds[pitch] = true;
  update = true;

  Serial.print("On: ");
  Serial.print(pitch);
  Serial.print("\n");
}

void ss_falling ()
{
  pos = 0;
  // Serial.println("position");
}  

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  
  pitch -= 21;
  leds[pitch] = false;
  updatedLeds[pitch] = true;
  update = true;


  Serial.print("Off: ");
  Serial.print(pitch);
  Serial.print("\n");
}


void MyCCFunction(byte channel, byte number, byte value) {


}
void setup() {
 
  received = false;
  pos = 0;
  pos2 = 0;
  process_it = false;
  // SPI.attachInterrupt();                  
  attachInterrupt (0, ss_falling, FALLING);

  Serial2.begin(31250, SERIAL_8N1, 18, 17);
  Serial.begin(115200);
  strip.Begin();
  strip.Show();

  
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.

  
  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(MyCCFunction); // This command tells the MIDI Library

  delay(1000);
  Serial.println("Heelo world");
  Serial.println(MIDI_CHANNEL_OMNI);

  strip.SetPixelColor(0, RgbwColor(HslColor(0.4, 0.4, 0.1)));

  strip.Show();
}

void loop() {
  MIDI.read();
  currentTime = millis();
  
  if(update && (currentTime - lastUpdate >= 50)){
    Serial.println("update");


    

    for (int i = 0; i < 88; i++){
      if(updatedLeds[i]){
    
        // Each note changes two LEDs: positions i*2 and i*2+1
        int ledIndex = i * 2;
    
        if(leds[i]){
          Serial.println("update led");
          Serial.println(ledIndex);
          // Note ON: Set both LEDs ON
          strip.SetPixelColor(ledIndex, RgbwColor(HslColor(0.4, 0.4, 0.5)));
          strip.SetPixelColor(ledIndex + 1, RgbwColor(HslColor(0.4, 0.4, 0.5)));
        } else {
          // Note OFF: Set both LEDs OFF
          strip.SetPixelColor(ledIndex, RgbwColor(HslColor(0.4, 0.4, 0)));
          strip.SetPixelColor(ledIndex + 1, RgbwColor(HslColor(0.4, 0.4, 0)));
        }
    
        updatedLeds[i] = false;
      }
    }

    strip.Show();
    lastUpdate = currentTime;
    update = false;
  }
}
