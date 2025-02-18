#include <NeoPixelBus.h>
#include <MIDI.h>  // Add Midi Library
unsigned long lastUpdate;
unsigned long currentTime;
bool update;
bool leds [88];
bool updatedLeds [88];


const uint16_t PixelCount = 172; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 4;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

//Create an instance of the library with default name, serial port and settings
MIDI_CREATE_DEFAULT_INSTANCE();
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  pitch -= 21;
  leds[pitch] = true;
  updatedLeds[pitch] = true;
  update = true;
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  pitch -= 21;
  leds[pitch] = false;
  updatedLeds[pitch] = true;
  update = true;
}


void MyCCFunction(byte channel, byte number, byte value) {
  switch (number) {
    case 20:
      // analogWrite(LEDPWM1, value * 2);
      break;
  }
}

void setup() {

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
  // OMNI sets it to listen to all channels.. MIDI.begin(2) would set it

  // Put only the name of the function
  MIDI.setHandleNoteOn(handleNoteOn);  

  // Do the same for NoteOffs
  MIDI.setHandleNoteOff(handleNoteOff);
  //pinMode (LED, OUTPUT); // Set Arduino board pin 13 to output
  // to respond to notes on channel 2 only.
  MIDI.setHandleControlChange(MyCCFunction); // This command tells the MIDI Library
  // the function you want to call when a Continuous Controller command
  // is received. In this case it's "MyCCFunction".
}

void loop() { // Main loop

  MIDI.read(); // Continuously check if Midi data has been received.

  currentTime = millis();

  if((currentTime - lastUpdate >= 100)){
    Serial.println("update");
    
    // for (int i = 0; i < 44; i++){

    //     if(leds[i] == 1){
    //       strip.SetPixelColor(i, RgbwColor(HslColor(0.4, 0.4, 0.4)));
    //     } else {
    //       strip.SetPixelColor(i, RgbwColor(HslColor(0.4, 0.4, 0)));
    //     }
    // }

    for (int i = 0; i < 30; i++){
      if(updatedLeds[i]){
        if(leds[i] == 1){
          strip.SetPixelColor(i, RgbwColor(HslColor(0.4, 0.4, 0.4)));
        } else {
          strip.SetPixelColor(i, RgbwColor(HslColor(0.4, 0.4, 0)));
        }
        updatedLeds[i] = false;
      }
    }

    strip.Show();
    lastUpdate = currentTime;
  }
}

// MyCCFunction is the function that will be called by the Midi Library
// when a Continuous Controller message is received.
// It will be passed bytes for Channel, Controller Number, and Value
// It checks if the controller number is within the 22 to 27 range
// If it is, light up the corresponding LED with the PWM brightness equal to the Value byte