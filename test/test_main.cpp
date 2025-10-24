#include <Arduino.h>
#include <unity.h>
#include "DCCRailcom.h"

// Define mock pins for testing
#define DCC_PIN_TEST 2
#define RAILCOM_PIN_TEST 3

DCCRailcom dcc(DCC_PIN_TEST, RAILCOM_PIN_TEST);

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_send_packet() {
    uint8_t rawPacket[] = {
        0b11111111, // Preamble
        0b11111111, // Preamble
        0b00000001, // 0 + Address (3) - first 7 bits
        0b10011101, // last bit of address + 0 + command (speed 58) - first 6 bits
        0b01001110, // last 2 bits of command + 0 + checksum (118) - first 5 bits
        0b11010000  // last 3 bits of checksum + 1 (end bit) + padding
    };
    int numBits = 44; // Total bits in the packet

    // This is a basic test and does not verify the timing of the DCC signal.
    // A more advanced test would require a logic analyzer or oscilloscope.
    dcc.sendPacket(rawPacket, numBits);

    TEST_ASSERT_EQUAL(digitalRead(RAILCOM_PIN_TEST), LOW);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_send_packet);
    UNITY_END();
}

void loop() {
}
