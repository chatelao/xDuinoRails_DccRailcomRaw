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

  // Construct the DCC packet without the checksum
  uint8_t dccPacket[] = {
    0xFF, 0xFF, // Preamble (16 bits of '1's)
    0x03,       // Address (loco 3)
    0x3F,       // Command (128-step speed)
    0x3A        // Speed (58)
  };

  // The number of bits in the packet to be sent, including start bits and preamble
  // Preamble (16) + Start (1) + Address (8) + Start (1) + Command (8) + Start (1) + Speed (8) = 43
  int numBits = 43;

  // Send the DCC packet and calculate the checksum from the last 3 bytes (address, command, speed)
  dcc.sendPacket(dccPacket, numBits, 3);

  // Wait for a second before sending the next packet
  delay(1000);
}
