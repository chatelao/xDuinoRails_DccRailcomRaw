#include "DCCRailcom.h"

// DCC timing constants in microseconds
#define DCC_ONE_PULSE 58
#define DCC_ZERO_PULSE 100

// Railcom cutout duration in microseconds
#define RAILCOM_CUTOUT_DURATION 484

/**
 * @brief Construct a new DCCRailcom object.
 * @param dccPin The pin to output the DCC signal on.
 * @param railcomPin The pin to output the Railcom cutout signal on.
 */
DCCRailcom::DCCRailcom(int dccPin, int railcomPin) {
  _dccPin = dccPin;
  _railcomPin = railcomPin;
}

/**
 * @brief Initialize the library and set the pin modes.
 */
void DCCRailcom::begin() {
  pinMode(_dccPin, OUTPUT);
  pinMode(_railcomPin, OUTPUT);
  digitalWrite(_dccPin, HIGH); // DCC idle state
  digitalWrite(_railcomPin, LOW); // Railcom cutout is normally off
}

/**
 * @brief Write a single bit to the DCC pin.
 * @param bit The bit to write (0 or 1).
 */
void DCCRailcom::writeBit(int bit) {
  int pulse = (bit == 1) ? DCC_ONE_PULSE : DCC_ZERO_PULSE;
  digitalWrite(_dccPin, LOW);
  delayMicroseconds(pulse);
  digitalWrite(_dccPin, HIGH);
  delayMicroseconds(pulse);
}

/**
 * @brief Send a DCC packet.
 * @param data A byte array containing the data to send.
 * @param numBits The number of bits to send from the data array.
 */
void DCCRailcom::sendPacket(const uint8_t* data, int numBits) {
  // 1. Railcom Cutout
  digitalWrite(_railcomPin, HIGH);
  delayMicroseconds(RAILCOM_CUTOUT_DURATION);
  digitalWrite(_railcomPin, LOW);

  // 2. Send the raw data packet
  for (int i = 0; i < numBits; i++) {
    int byteIndex = i / 8;
    int bitIndex = 7 - (i % 8); // MSB first
    int bit = (data[byteIndex] >> bitIndex) & 1;
    writeBit(bit);
  }
}
