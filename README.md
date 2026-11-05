# Arduino Braille Translator

**A touch-based Braille-to-text translation system built with Arduino Nano, capacitive sensors, and an OLED display.**

## Overview

The Arduino Braille Translator is an embedded systems project designed to convert six-dot Braille patterns into readable text.

The system uses six capacitive touch sensors arranged to represent a standard Braille cell. An Arduino Nano processes the sensor signals, identifies the corresponding character using binary pattern matching, and displays the translated text on an OLED screen.

The project explores accessible human-computer interaction, digital signal processing, and microcontroller-based input systems.

## Features

- **Braille Character Recognition:** Decodes the 26 letters of the English alphabet using six-bit sensor patterns.
- **Touch-Based Input:** Uses six TTP223B capacitive touch sensors to detect combinations.
- **Real-Time Feedback:** Displays the detected letter and a visual hold-duration indicator.
- **Text Editing:** Supports character insertion, spaces, backspace, and text reset.
- **OLED Output:** Displays translated text using a compact screen.
- **Memory Management:** Uses a fixed-size character buffer without dynamic memory allocation.

## Hardware Components

| Component | Description |
|---|---|
| Arduino Nano | ATmega328P-based microcontroller |
| 6 × TTP223B | Capacitive touch sensors |
| OLED Display | Text output and visual feedback |
| Arduino Nano Expansion Shield | Simplifies hardware connections |
| Jumper Wires | Electrical connections |

## How It Works

### 1. Braille Input

A standard Braille cell consists of six dots arranged in two columns and three rows.

Each touch sensor represents one Braille dot. The Arduino reads the sensors as digital inputs and constructs a six-bit binary pattern.

```cpp
uint8_t pattern = 0;

for (uint8_t i = 0; i < 6; i++) {
    if (digitalRead(SENZORI[i]) == HIGH) {
        pattern |= (1 << i);
    }
}
```

### 2. Character Decoding

The generated binary pattern is compared against predefined Braille combinations using the `decodeBraille()` function.

For example:

```cpp
case 0b000001: return 'A';
case 0b000011: return 'B';
case 0b001001: return 'C';
```

The recognized character is then added to the text buffer when the appropriate input duration is detected.

### 3. Duration-Based Controls

The system uses the duration of the touch interaction to determine the desired operation.

| Hold Duration | Operation |
|---|---|
| Less than 1 second | No text operation |
| 1–2 seconds | Insert recognized letter |
| 2–3 seconds | Insert space |
| 3–4 seconds | Delete last character |
| 4 seconds or longer | Clear text |

The operation is executed after all sensors are released.

### 4. OLED Display

The OLED displays the translated text, the current recognized character, and a visual indicator of the hold duration.

The implementation stores up to 64 characters and displays up to three rows of 16 characters.

## Hardware Connections

The six sensors are connected to the following Arduino Nano digital pins:

| Sensor | Arduino Pin |
|---|---|
| Sensor 1 | D2 |
| Sensor 2 | D3 |
| Sensor 3 | D5 |
| Sensor 4 | D11 |
| Sensor 5 | D12 |
| Sensor 6 | D13 |

Each sensor also requires VCC and GND connections.

The OLED uses the I2C interface:

| OLED | Arduino Nano |
|---|---|
| SDA | A4 |
| SCL | A5 |
| VCC | VCC |
| GND | GND |

## Software and Technologies

- Arduino IDE
- Embedded C/C++
- Arduino Nano / ATmega328P
- `Tiny4kOLED` library
- Digital GPIO input
- Bitwise operations
- Timing with `millis()`
- I2C communication

## Getting Started

### Requirements

- Arduino IDE
- Arduino Nano board
- Six capacitive touch sensors
- Compatible OLED display
- `Tiny4kOLED` library

## Testing and Results

The prototype was tested using combinations of capacitive touch inputs.

The project documentation reports successful recognition of Braille patterns, correct character display, and functioning duration-based editing controls.

The OLED interface provides visual feedback while the user interacts with the sensors.

## Limitations and Future Improvements

The current implementation focuses primarily on alphabetic Braille input.

Potential extensions include:

- Full numeric and punctuation support.
- Improved input stability and character recognition.
- Support for longer text and scrolling.
- A larger display for improved readability.
- Additional accessibility features, including alternative feedback methods.

## Documentation

The complete project report, including the hardware design, circuit schematic, software explanation, and experimental results, is available here:

*Note: The circuit diagram in the report uses substitute components for simulation, including an Arduino Uno and push buttons.*

## Project Context

The project demonstrates practical applications of embedded programming, sensor integration, digital input processing, and assistive technology.

