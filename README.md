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

The `DCCRailcom` library is simple to use. It sends a raw bitstream to the DCC pin, giving you complete control over the signal. You are responsible for constructing the entire DCC packet, including the preamble, start bits, address, command, speed, checksum, and end bit.

Here's a quick overview of the main functions:

- `DCCRailcom(dccPin, railcomPin)`: The constructor takes two arguments: the pin for the DCC signal and the pin for the Railcom cutout.
- `begin()`: Initializes the library and sets the pin modes.
- `sendPacket(data, numBits)`: Sends a DCC packet. The `data` argument is a byte array containing the raw packet, and `numBits` is the number of bits to send.

### Example: Sending a Speed Command

The following example demonstrates how to send a 128-step speed command to a locomotive with address 3, setting the speed to 58.

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
  // --- DCC Packet Structure (128-step speed control) ---
  // A standard DCC packet for 128-step speed control consists of:
  // 1. Preamble: 12-14 '1' bits
  // 2. Start Bit: '0'
  // 3. Address Byte: 8 bits for the locomotive address
  // 4. Start Bit: '0'
  // 5. Command Byte: 8 bits (00111111 for 128-step speed)
  // 6. Start Bit: '0'
  // 7. Speed Byte: 8 bits (0xxxxxxx, where xxxxxxx is the speed)
  // 8. Start Bit: '0'
  // 9. Checksum Byte: 8 bits (XOR of address, command, and speed bytes)
  // 10. End Bit: '1'

  // --- Example: Send Speed 58 to Loco 3 ---
  // Address: 3 (00000011)
  // Command: 128-step speed (00111111)
  // Speed: 58 (00111010)
  // Checksum: 3 XOR 63 XOR 58 = 10 (00001010)

  // Construct the complete DCC packet as a raw binary array
  // The bitstream is:
  // 11111111 11111111 0 00000011 0 00111111 0 00111010 0 00001010 1
  uint8_t rawPacket[] = {
    0xFF,       // Preamble
    0xFC,       // Preamble (4 bits) + 0 + Address (3) - first 3 bits
    0x1F,       // last 5 bits of address + 0 + Command (63) - first 2 bits
    0xDE,       // last 6 bits of command + 0 + Speed (58) - first 1 bit
    0x82,       // last 7 bits of speed + 0
    0x8A,       // Checksum (10)
    0x80        // End bit (1) + 7 padding bits
  };

  // The number of bits in the packet to be sent
  // Preamble (16) + Start (1) + Address (8) + Start (1) + Command (8) + Start (1) + Speed (8) + Start (1) + Checksum (8) + End (1) = 52
  int numBits = 52;

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
