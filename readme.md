

# Lighting Piano Keys with an LED Strip and ESP32

Check out my YouTube channel. [Marek Mráz - Music](https://www.youtube.com/@MarekMrazMusic)

https://github.com/user-attachments/assets/6d4d9031-3474-4080-8b39-3a847859fb8f



![circuit](image.png)


This project creates synchronized visual effects on a piano by illuminating an LED strip based on MIDI data. The system translates real-time MIDI messages from a piano into dynamic lighting patterns. 


LED used: SK6812 RGBW Led Strip 5V (0.3 watt per LED)



## How It Works

- **MIDI Data Acquisition**  
  A piano sends MIDI data through its output. These messages, which include note on/off events and control changes, are received by the microcontroller via a MIDI interface.

- **Data Processing and LED Control**  
  The received MIDI messages are parsed and processed using a MIDI library implemented in C++. Corresponding LED states on the strip are updated based on the MIDI input. The NeoPixelBus library then drives the LED strip, lighting up 88 LEDs to represent the piano keys.

- **Power Supply Considerations**  
  It depends on LED strip power consumption.

## Software and Libraries

The project is implemented in C++ using the Arduino framework. Two main libraries are used:

- **MIDI Library**  
  Handles reading, parsing, and processing of MIDI messages.

- **NeoPixelBus Library**  
  Provides efficient control of LED strips, translating LED state changes into visible effects.

## Schematic

The wiring and connections for the system are documented in the project schematic shown below:

![Schematic](images/midi_esp32.png)
