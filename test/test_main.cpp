#include <ArduinoFake.h>
#include <unity.h>
#include <DCCRailcom.h>

using namespace fakeit;

// Define mock pins for testing
#define DCC_PIN_TEST 2
#define RAILCOM_PIN_TEST 3

DCCRailcom dcc(DCC_PIN_TEST, RAILCOM_PIN_TEST);

void setUp(void) {
    ArduinoFakeReset();
}

void tearDown(void) {
    // clean stuff up here
}

void test_send_packet() {
    // Bitstream: 11111111 11111111 0 00000011 0 00111111 0 00111010 0 00001010 1
    uint8_t rawPacket[] = {
        0xFF, // Preamble (Byte 1)
        0xFF, // Preamble (Byte 2)
        0x01, // 0 (Start) + 0000001 (Addr bits 7-1)
        0x8F, // 1 (Addr bit 0) + 0 (Start) + 001111 (Cmd bits 7-2)
        0xC7, // 11 (Cmd bits 1-0) + 0 (Start) + 00111 (Speed bits 7-3)
        0x40, // 010 (Speed bits 2-0) + 0 (Start) + 0000 (Checksum bits 7-4)
        0xA8  // 1010 (Checksum bits 3-0) + 1 (End) + 000 (Padding)
    };
    int numBits = 53;

    // Stub the functions that will be called by the library
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delayMicroseconds)).AlwaysReturn();

    // This test verifies that the correct number of bits are sent and that the
    // Railcom pin is cycled correctly. It does not verify the timing of the
    // DCC signal, which would require a logic analyzer or oscilloscope.
    dcc.sendPacket(rawPacket, numBits);

    // Verify that digitalWrite was called the correct number of times
    Verify(Method(ArduinoFake(), digitalWrite)).Exactly(53 * 2 + 2);

    // Verify that the railcom pin was cycled
    Verify(Method(ArduinoFake(), digitalWrite).Using(RAILCOM_PIN_TEST, HIGH)).Once();
    Verify(Method(ArduinoFake(), digitalWrite).Using(RAILCOM_PIN_TEST, LOW)).Once();
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_send_packet);
    UNITY_END();
    return 0;
}
