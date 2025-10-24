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
    // --- Example: Send Speed 58 to Loco 3 ---
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
    int numBits = 52;

    // This test verifies that the correct number of bits are sent and that the
    // Railcom pin is cycled correctly. It does not verify the timing of the
    // DCC signal, which would require a logic analyzer or oscilloscope.
    dcc.sendPacket(rawPacket, numBits);

    // Verify that digitalWrite was called the correct number of times
    Verify(Method(ArduinoFake(), digitalWrite)).Exactly(52 * 2 + 2);

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
