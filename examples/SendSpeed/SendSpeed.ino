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
