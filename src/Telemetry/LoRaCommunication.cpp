
#include "LoRaCommunication.hpp"

#include <RadioLib.h>

#include "PinConfiguration.hpp"
#include "Logging/SystemLogger.hpp"
#include "Utility/UtilityFunctions.hpp"

LoRaCommunication::LoRaCommunication(SPIClass & spi)
        : radio(new Module(LoRa_CS, LoRa_DIO0, LoRa_RST, RADIOLIB_NC, spi))
{
    UtilityFunctions::EnableLoRaSPI();

    // initialize SX1278 with default settings
    Serial.print(F("[RFM95W] Initializing ... "));
    // int state = radio.begin(868.0, 125.0, 9, 7, RADIOLIB_SX127X_SYNC_WORD, 10, 8, 0);
    int state = radio.begin();
    delay(1);
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println(F("success!"));
    } else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
    }

    UtilityFunctions::DisableLoRaSPI();
}

bool LoRaCommunication::transmit(const char * str)
{
    UtilityFunctions::EnableLoRaSPI();

    int state = radio.transmit(str);
    if (state == RADIOLIB_ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F(" success!"));

        // print measured data rate
        Serial.print(F("[RFM95W] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));

        UtilityFunctions::DisableLoRaSPI();

        return true;

    } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));

    } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
        // timeout occurred while transmitting packet
        Serial.println(F("timeout!"));

    } else {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);

    }

    UtilityFunctions::DisableLoRaSPI();

    return false;
}

//void setup() {
//    Serial.begin(115200);
//    SPI.begin(SPI1_SCK, SPI1_MISO, SPI1_MOSI);
//
//    // initialize SX1278 with default settings
//    Serial.print(F("[RFM95W] Initializing ... "));
//    int state = radio.begin(868.0, 125.0, 9, 7, RADIOLIB_SX127X_SYNC_WORD, 10, 8, 0);
//    if (state == RADIOLIB_ERR_NONE) {
//        Serial.println(F("success!"));
//    } else {
//        Serial.print(F("failed, code "));
//        Serial.println(state);
//    }
//
//    // some modules have an external RF switch
//    // controlled via two pins (RX enable, TX enable)
//    // to enable automatic control of the switch,
//    // call the following method
//    // RX enable:   4
//    // TX enable:   5
//    /*
//      radio.setRfSwitchPins(4, 5);
//    */
//}
//
//int counter = 0;
//char buf[8];
//
//void loop() {
//    Serial.print(F("[SX1278] Transmitting packet ... "));
//
//    // you can transmit C-string or Arduino string up to
//    // 256 characters long
//    // NOTE: transmit() is a blocking method!
//    //       See example SX127x_Transmit_Interrupt for details
//    //       on non-blocking transmission method.
//
//    snprintf(buf, sizeof (buf), "%d", counter);
//    counter++;
//
//    Serial.print("Transmitting:");
//    Serial.println(buf);
//
//    int state = radio.transmit(buf);
//
//    // you can also transmit byte array up to 256 bytes long
//    /*
//      byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
//      int state = radio.transmit(byteArr, 8);
//    */
//
//    if (state == RADIOLIB_ERR_NONE) {
//        // the packet was successfully transmitted
//        Serial.println(F(" success!"));
//
//        // print measured data rate
//        Serial.print(F("[SX1278] Datarate:\t"));
//        Serial.print(radio.getDataRate());
//        Serial.println(F(" bps"));
//
//    } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
//        // the supplied packet was longer than 256 bytes
//        Serial.println(F("too long!"));
//
//    } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
//        // timeout occurred while transmitting packet
//        Serial.println(F("timeout!"));
//
//    } else {
//        // some other error occurred
//        Serial.print(F("failed, code "));
//        Serial.println(state);
//
//    }
//
//    // wait for a second before transmitting again
//    delay(1000);
//}
