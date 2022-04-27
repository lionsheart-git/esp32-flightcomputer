/**
 * @file PinConfiguration.hpp
 * @brief Default pin configuration used throughout the program.
 *
 * Could later be done through arguments to give full versatility.
 */
#ifndef FLIGHTCOMPUTER_PINCONFIGURATION_HPP
#define FLIGHTCOMPUTER_PINCONFIGURATION_HPP

#define LoRa_SCK 5 /**< @brief Used by LoRa module */
#define LoRa_CS 18 /**< @brief Used by LoRa module */
#define LoRa_MISO 19 /**< @brief Used by LoRa module */
#define LoRa_MOSI 27 /**< @brief Used by LoRa module */
#define LoRa_DIO0 26 /**< @brief Used by LoRa module */
#define LoRa_RST 14 /**< @brief Used by LoRa module */

#define SD_SCK 13 /**< @brief SD clock pin */
#define SD_CS 27 /**< @brief SD chip select pin */
#define SD_MISO 12 /**< @brief SD miso pin */
#define SD_MOSI 14 /**< @brief SD mosi pin */

#define GPS_RX 32 /**< @brief GPS rx pin */
#define GPS_TX 33 /**<@brief GPS tx pin */

#define I2C_SDA1 22
#define I2C_SCL1 21

#define I2C_SDA2 25
#define I2C_SCL2 26

#endif //FLIGHTCOMPUTER_PINCONFIGURATION_HPP
