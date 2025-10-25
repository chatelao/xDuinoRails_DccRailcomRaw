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
  // --- DCC Packet for Accessory Decoder ---
  // Address:                     22
  // Straight (closed):           State 0
  // Curved (thrown):             State 1

  // --- Packet for Turnout 22 Straight ---
  uint8_t straightPacket[] = {
    0xFF, // Preamble
    0xFF, // Preamble
    0x86, // Address Byte for decoder 6
    0x8A, // Data Byte for subaddress 1, state 0
    0x0C  // Checksum
  };

  // --- Packet for Turnout 22 Curved ---
  uint8_t curvedPacket[] = {
    0xFF, // Preamble
    0xFF, // Preamble
    0x86, // Address Byte for decoder 6
    0x8B, // Data Byte for subaddress 1, state 1
    0x0D  // Checksum
  };

  // The number of bits in the packet to be sent
  int numBits = 44;

  // Send the packet to set the turnout to straight
  dcc.sendPacket(straightPacket, numBits);

  // Wait for 2 seconds
  delay(2000);

  // Send the packet to set the turnout to curved
  dcc.sendPacket(curvedPacket, numBits);

  // Wait for 2 seconds
  delay(2000);
}
