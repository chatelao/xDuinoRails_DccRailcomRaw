#include <DCCRailcom.h>

// Define the pins for the DCC signal and Railcom cutout
#define DCC_PIN 2
#define RAILCOM_PIN 3

// Create a DCCRailcom object
DCCRailcom dcc(DCC_PIN, RAILCOM_PIN);

void setup() {
  // Initialize the DCCRailcom library
  Serial.begin(9600);
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
  // Address:                     3 (00000011)
  // Command: 128-step speed (63)   (00111111)
  // Speed:                      58 (00111010)
  // Checksum: 3 XOR 63 XOR 58 = 10 (00001010)

  // --- Bitstream Construction ---
  // The DCC bitstream is constructed by concatenating the following:
  // Preamble (16 bits)
  // Start Bit (1 bit)
  // Address (8 bits)
  // Start Bit (1 bit)
  // Command (8 bits)
  // Start Bit (1 bit)
  // Speed (8 bits)
  // Start Bit (1 bit)
  // Checksum (8 bits)
  // End Bit (1 bit)
  // Total bits: 53
  //
  // Bitstream: 11111111 11111111 0 00000011 0 00111111 0 00111010 0 00001010 1
  //
  // This bitstream is then chunked into 8-bit segments to form the byte array.
  // Note: The last byte is padded with trailing zeros to make a full byte.
  uint8_t rawPacket[] = {
    0b11111111, // Preamble (Byte 1)
    0b11111111, // Preamble (Byte 2)
    0b00000001, // 0 (Start) + 0000001 (Addr bits 7-1)
    0b10001111, // 1 (Addr bit 0) + 0 (Start) + 001111 (Cmd bits 7-2)
    0b11000111, // 11 (Cmd bits 1-0) + 0 (Start) + 00111 (Speed bits 7-3)
    0b01000000, // 010 (Speed bits 2-0) + 0 (Start) + 0000 (Checksum bits 7-4)
    0b10101000  // 1010 (Checksum bits 3-0) + 1 (End) + 000 (Padding)
  };

  // The number of bits in the packet to be sent
  int numBits = 53;

  // Send the DCC packet
  dcc.sendPacket(rawPacket, numBits);

  // Wait for a second before sending the next packet
  delay(1000);
}
