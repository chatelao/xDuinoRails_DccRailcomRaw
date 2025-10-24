# DCCRailcom Arduino Library

[![License: AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

An Arduino library for generating DCC (Digital Command Control) signals with a Railcom cutout. This library is designed to be lightweight and flexible, allowing you to send raw DCC packets to your model railroad layout.

## Features

- Generates a DCC signal on a user-defined pin.
- Creates a Railcom cutout on a separate, user-defined pin.
- Accepts DCC packets as a raw binary array, giving you full control over the data being sent.
- No limitations on the length of the binary array, allowing for custom preamble lengths.
- Compatible with the XIAO Seed RP2040 and other Arduino-compatible boards.
- Includes PlatformIO support for easy project management.

## Installation

### Arduino IDE

1. Download the latest release from the [GitHub repository](https://github.com/jules-labs/DCCRailcom/releases).
2. In the Arduino IDE, go to `Sketch` -> `Include Library` -> `Add .ZIP Library...` and select the downloaded file.
3. The library will now be available in the `Include Library` menu.

### PlatformIO

1. Add the following line to your `platformio.ini` file:
   ```ini
   lib_deps = jules-labs/DCCRailcom
   ```
2. PlatformIO will automatically download and install the library.

## Usage

The `DCCRailcom` library is simple to use. Here's a quick overview of the main functions:

- `DCCRailcom(dccPin, railcomPin)`: The constructor takes two arguments: the pin for the DCC signal and the pin for the Railcom cutout.
- `begin()`: Initializes the library and sets the pin modes.
- `sendPacket(data, numBits)`: Sends a DCC packet. The `data` argument is a byte array containing the raw packet, and `numBits` is the number of bits to send.

### Example: Sending a Speed Command

The following example demonstrates how to send a speed command to a locomotive with address 3, setting the speed to 58.

```cpp
#include <DCCRailcom.h>

// Define the pins for the DCC signal and Railcom cutout
#define DCC_PIN 2
#define RAILCOM_PIN 3

// Create a DCCRailcom object
DCCRailcom dcc(DCC_PIN, RAILCOM_PIN);

void setup() {
  // Initialize the DCCRailcom library
  dcc.begin();
}

void loop() {
  // Construct the DCC packet as a raw binary array
  // This packet sends speed 58 to locomotive 3
  uint8_t rawPacket[] = {
      0b11111111, // Preamble
      0b11111111, // Preamble
      0b00000001, // 0 + Address (3) - first 7 bits
      0b10011101, // last bit of address + 0 + command (speed 58) - first 6 bits
      0b01001110, // last 2 bits of command + 0 + checksum (118) - first 5 bits
      0b11010000  // last 3 bits of checksum + 1 (end bit) + padding
  };

  int numBits = 44; // Total bits in the packet

  // Send the DCC packet
  dcc.sendPacket(rawPacket, numBits);

  // Wait for a second before sending the next packet
  delay(1000);
}
```

## Contributing

Contributions are welcome! Please open an issue or submit a pull request on the [GitHub repository](https://github.com/jules-labs/DCCRailcom).

## License

This library is licensed under the [AGPL-3.0 License](LICENSE).
