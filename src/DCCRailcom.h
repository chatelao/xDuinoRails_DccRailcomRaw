#ifndef DCCRailcom_h
#define DCCRailcom_h

#include "Arduino.h"

/**
 * @class DCCRailcom
 * @brief A class to generate DCC signals with a Railcom cutout.
 */
class DCCRailcom {
public:
  /**
   * @brief Construct a new DCCRailcom object.
   * @param dccPin The pin to output the DCC signal on.
   * @param railcomPin The pin to output the Railcom cutout signal on.
   */
  DCCRailcom(int dccPin, int railcomPin);

  /**
   * @brief Initialize the library and set the pin modes.
   */
  void begin();

  /**
   * @brief Send a DCC packet.
   * @param data A byte array containing the data to send.
   * @param numBits The number of bits to send from the data array.
   */
  void sendPacket(const uint8_t* data, int numBits);

private:
  int _dccPin;
  int _railcomPin;

  /**
   * @brief Write a single bit to the DCC pin.
   * @param bit The bit to write (0 or 1).
   */
  void writeBit(int bit);
};

#endif
