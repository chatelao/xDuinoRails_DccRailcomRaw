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
  // The bitstream is constructed to control turnout 22.
  // This corresponds to decoder address 6, output pair 1.
  uint8_t straightPacket[] = {
    0xFF, // Preamble (11111111)
    0xFF, // Preamble (11111111)
    0x86, // Address Byte for decoder 6 (10000110)
    0x8A, // Data Byte for subaddress 1, state 0 (10001010)
    0x0C  // Checksum (00001100)
  };

  // --- Packet for Turnout 22 Curved ---
  uint8_t curvedPacket[] = {
    0xFF, // Preamble (11111111)
    0xFF, // Preamble (11111111)
    0x86, // Address Byte for decoder 6 (10000110)
    0x8B, // Data Byte for subaddress 1, state 1 (10001011)
    0x0D  // Checksum (00001101)
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
