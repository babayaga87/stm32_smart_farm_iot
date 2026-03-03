/**
 * @file nrf24l01.h
 * @brief nRF24L01 Wireless Transceiver Driver for STM32
 * @author Trần Minh Khải
 * @date 18/9/25
 * @version 1.0
 *
 * This library provides a comprehensive driver for the nRF24L01 2.4GHz wireless
 * transceiver module. It supports all major features including multi-pipe communication,
 * auto-acknowledgment, dynamic payload lengths, and various power/data rate configurations.
 *
 * @note This driver is designed for STM32 microcontrollers using HAL library
 *
 * @par Example Usage:
 * @code
 * // Initialize device with default configuration
 * nrf24l01_device nrf = nrf24l01_get_default_config();
 * nrf.spi = &hspi1;
 * nrf.ce_port = GPIOA;
 * nrf.ce_pin = 8;
 * nrf.csn_port = GPIOA;
 * nrf.csn_pin = 9;
 *
 * // Initialize the device
 * if (nrf24l01_init(&nrf) == 0) {
 *     // Configure for transmission
 *     uint8_t tx_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
 *     nrf24l01_data_pipe_address(&nrf, 0, tx_addr, 5);
 *     nrf24l01_transmit(&nrf);
 *
 *     // Send data
 *     uint8_t data[] = "Hello World";
 *     nrf24l01_write_tx_payload(&nrf, data, sizeof(data));
 * }
 * @endcode
 */

#ifndef NRF24L01_DRIVER_NRF24L01_H
#define NRF24L01_DRIVER_NRF24L01_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "stm32f1xx_hal.h"

/**
 * @defgroup NRF24L01_MACROS Macros and Constants
 * @brief Predefined macros and constant values
 * @{
 */

/** @brief Empty data placeholder for uninitialized pointers */
#define EMPTY_DATA   (uint8_t *)0xFF

/** @} */ // End of NRF24L01_MACROS group

/**
 * @defgroup NRF24L01_REGISTERS Register Addresses
 * @brief nRF24L01 register memory map addresses
 * @{
 */

#define CONFIG        0x00  /**< Configuration Register */
#define EN_AA         0x01  /**< Enable 'Auto Acknowledgment' Function */
#define EN_RXADDR     0x02  /**< Enabled RX Addresses */
#define SETUP_AW      0x03  /**< Setup of Address Widths */
#define SETUP_RETR    0x04  /**< Setup of Automatic Retransmission */
#define RF_CH         0x05  /**< RF Channel */
#define RF_SETUP      0x06  /**< RF Setup Register */
#define STATUS        0x07  /**< Status Register */
#define OBSERVE_TX    0x08  /**< Transmit observe register */
#define PRD           0x09  /**< Power Detection Register */
#define RX_ADDR_P0    0x0A  /**< Receive address data pipe 0 */
#define RX_ADDR_P1    0x0B  /**< Receive address data pipe 1 */
#define RX_ADDR_P2    0x0C  /**< Receive address data pipe 2 */
#define RX_ADDR_P3    0x0D  /**< Receive address data pipe 3 */
#define RX_ADDR_P4    0x0E  /**< Receive address data pipe 4 */
#define RX_ADDR_P5    0x0F  /**< Receive address data pipe 5 */
#define TX_ADDR       0x10  /**< Transmit address */
#define RX_PW_P0      0x11  /**< Number of bytes in RX payload in data pipe 0 */
#define RX_PW_P1      0x12  /**< Number of bytes in RX payload in data pipe 1 */
#define RX_PW_P2      0x13  /**< Number of bytes in RX payload in data pipe 2 */
#define RX_PW_P3      0x14  /**< Number of bytes in RX payload in data pipe 3 */
#define RX_PW_P4      0x15  /**< Number of bytes in RX payload in data pipe 4 */
#define RX_PW_P5      0x16  /**< Number of bytes in RX payload in data pipe 5 */
#define FIFO_STATUS   0x17  /**< FIFO Status Register */
#define DYNPD         0x1C  /**< Enable dynamic payload length */
#define FEATURE       0x1D  /**< Feature Register */

/** @} */ // End of NRF24L01_REGISTERS group

/**
 * @defgroup NRF24L01_BITMASKS Register Bit Masks
 * @brief Bit field definitions for nRF24L01 registers
 * @{
 */

/* CONFIG Register Bits */
#define MASK_RX_DR    0b01000000  /**< Mask interrupt caused by RX_DR */
#define MASK_TX_DS    0b00100000  /**< Mask interrupt caused by TX_DS */
#define MASK_MAX_RT   0b00010000  /**< Mask interrupt caused by MAX_RT */
#define EN_CRC        0b00001000  /**< Enable CRC */
#define CRCO          0b00000100  /**< CRC encoding scheme */
#define PWR_UP        0b00000010  /**< Power UP */
#define PRIM_RX       0b00000001  /**< Primary RX/TX control */

/* EN_AA Register Bits */
#define ENAA_P5       0b00100000  /**< Enable auto acknowledgement data pipe 5 */
#define ENAA_P4       0b00010000  /**< Enable auto acknowledgement data pipe 4 */
#define ENAA_P3       0b00001000  /**< Enable auto acknowledgement data pipe 3 */
#define ENAA_P2       0b00000100  /**< Enable auto acknowledgement data pipe 2 */
#define ENAA_P1       0b00000010  /**< Enable auto acknowledgement data pipe 1 */
#define ENAA_P0       0b00000001  /**< Enable auto acknowledgement data pipe 0 */

/* EN_RXADDR Register Bits */
#define ERX_P5        0b00100000  /**< Enable data pipe 5 */
#define ERX_P4        0b00010000  /**< Enable data pipe 4 */
#define ERX_P3        0b00001000  /**< Enable data pipe 3 */
#define ERX_P2        0b00000100  /**< Enable data pipe 2 */
#define ERX_P1        0b00000010  /**< Enable data pipe 1 */
#define ERX_P0        0b00000001  /**< Enable data pipe 0 */

/* SETUP_AW Register Bits */
#define AW            0b00000011  /**< RX/TX Address field width */
#define AW0           0b00000001  /**< Address width bit 0 */
#define AW1           0b00000010  /**< Address width bit 1 */

/* SETUP_RETR Register Bits */
#define ARD           0b11110000  /**< Auto Retransmit Delay */
#define ARD0          0b00010000  /**< Auto Retransmit Delay bit 0 */
#define ARD1          0b00100000  /**< Auto Retransmit Delay bit 1 */
#define ARD2          0b01000000  /**< Auto Retransmit Delay bit 2 */
#define ARD3          0b10000000  /**< Auto Retransmit Delay bit 3 */
#define ARC           0b00001111  /**< Auto Retransmit Count */
#define ARC0          0b00000001  /**< Auto Retransmit Count bit 0 */
#define ARC1          0b00000010  /**< Auto Retransmit Count bit 1 */
#define ARC2          0b00000100  /**< Auto Retransmit Count bit 2 */
#define ARC3          0b00001000  /**< Auto Retransmit Count bit 3 */

/* RF_CH Register Bits */
#define RF_CH0        0b00000001  /**< RF channel frequency bit 0 */
#define RF_CH1        0b00000010  /**< RF channel frequency bit 1 */
#define RF_CH2        0b00000100  /**< RF channel frequency bit 2 */
#define RF_CH3        0b00001000  /**< RF channel frequency bit 3 */
#define RF_CH4        0b00010000  /**< RF channel frequency bit 4 */
#define RF_CH5        0b00100000  /**< RF channel frequency bit 5 */
#define RF_CH6        0b01000000  /**< RF channel frequency bit 6 */

/* RF_SETUP Register Bits */
#define PLL_LOCK      0b00010000  /**< Force PLL lock signal */
#define RF_DR         0b00001000  /**< Air Data Rate */
#define RF_PWR        0b00000110  /**< Set RF output power in TX mode */
#define RF_PWR0       0b00000010  /**< RF output power bit 0 */
#define RF_PWR1       0b00000100  /**< RF output power bit 1 */
#define LNA_HCURR     0b00000001  /**< Setup LNA gain */

/* STATUS Register Bits */
#define RX_DR         0b01000000  /**< Data Ready RX FIFO interrupt */
#define TX_DS         0b00100000  /**< Data Sent TX FIFO interrupt */
#define MAX_RT        0b00010000  /**< Maximum number of TX retransmits interrupt */
#define RX_P_NO       0b00001110  /**< Data pipe number for available payload */
#define RX_P_NO0      0b00000010  /**< Data pipe number bit 0 */
#define RX_P_NO1      0b00000100  /**< Data pipe number bit 1 */
#define RX_P_NO2      0b00001000  /**< Data pipe number bit 2 */
#define TX_FULL       0b00000001  /**< TX FIFO full flag */

/* OBSERVE_TX Register Bits */
#define PLOS_CNT      0b11110000  /**< Count lost packets */
#define PLOS_CNT0     0b00010000  /**< Packet lost count bit 0 */
#define PLOS_CNT1     0b00100000  /**< Packet lost count bit 1 */
#define PLOS_CNT2     0b01000000  /**< Packet lost count bit 2 */
#define PLOS_CNT3     0b10000000  /**< Packet lost count bit 3 */
#define ARC_CNT       0b00001111  /**< Count retransmitted packets */
#define ARC_CNT0      0b00000001  /**< Auto retransmit count bit 0 */
#define ARC_CNT1      0b00000010  /**< Auto retransmit count bit 1 */
#define ARC_CNT2      0b00000100  /**< Auto retransmit count bit 2 */
#define ARC_CNT3      0b00001000  /**< Auto retransmit count bit 3 */

/* CD Register Bits */
#define CD            0b00000001  /**< Carrier Detect */

/* FIFO_STATUS Register Bits */
#define TX_REUSE      0b01000000  /**< Used for a PTX device */
#define FIFO_FULL     0b00100000  /**< TX FIFO full flag */
#define TX_EMPTY      0b00010000  /**< TX FIFO empty flag */
#define RX_FULL       0b00000010  /**< RX FIFO full flag */
#define RX_EMPTY      0b00000001  /**< RX FIFO empty flag */

/* FEATURE Register Bits */
#define EN_DPL        0b00000100  /**< Enable Dynamic Payload Length */
#define EN_ACK_PAY    0b00000010  /**< Enable Payload with ACK */
#define EN_DYN_ACK    0b00000001  /**< Enable the W_TX_PAYLOAD_NOACK command */

/** @} */ // End of NRF24L01_BITMASKS group

/**
 * @defgroup NRF24L01_COMMANDS SPI Commands
 * @brief Command definitions for SPI communication
 * @{
 */

#define R_REGISTER    0x00  /**< Read command and status registers */
#define W_REGISTER    0x20  /**< Write command and status registers */
#define R_RX_PAYLOAD  0x61  /**< Read RX payload */
#define W_TX_PAYLOAD  0xA0  /**< Write TX payload */
#define FLUSH_TX      0xE1  /**< Flush TX FIFO */
#define FLUSH_RX      0xE2  /**< Flush RX FIFO */
#define REUSE_TX_PL   0xE3  /**< Reuse last transmitted payload */
#define ACTIVATE      0x50  /**< Activate features */
#define R_RX_PL_WID   0x60  /**< Read RX payload width */
#define W_ACK_PAYLOAD 0xA8  /**< Write payload to be transmitted with ACK packet (bit[2:0] specify pipe) */
#define W_TX_PAYLOAD_NOACK   0xB0  /**< Write TX payload and disable AUTOACK */
#define NOP           0xFF  /**< No operation, might be used to read status register */

/** @} */ // End of NRF24L01_COMMANDS group

/**
 * @defgroup NRF24L01_ENUMS Enumeration Types
 * @brief Enumerated types for configuration options
 * @{
 */

/**
 * @brief Air data rate configuration
 */
typedef enum {
    nrf24l01_air_data_rate_1mbps = 0,  /**< 1 Mbps data rate */
    nrf24l01_air_data_rate_2mbps,      /**< 2 Mbps data rate */
} nrf24l01_air_data_rate;

/**
 * @brief RF output power levels
 */
typedef enum {
    nrf24l01_rf_output_power_minus18dbm = 0,  /**< -18 dBm output power */
    nrf24l01_rf_output_power_minus12dbm,      /**< -12 dBm output power */
    nrf24l01_rf_output_power_minus6dbm,       /**< -6 dBm output power */
    nrf24l01_rf_output_power_0dbm             /**< 0 dBm output power (maximum) */
} nrf24l01_rf_output_power;

/**
 * @brief Address width configuration
 */
typedef enum{
    nrf24l01_address_width_3bytes = 3,  /**< 3 bytes address width */
    nrf24l01_address_width_4bytes,      /**< 4 bytes address width */
    nrf24l01_address_width_5bytes,      /**< 5 bytes address width */
} nrf24l01_address_width;

/**
 * @brief Auto retransmit delay settings
 */
typedef enum{
    nrf24l01_auto_retransmit_delay_250us = 0,   /**< 250 µs delay */
    nrf24l01_auto_retransmit_delay_500us,       /**< 500 µs delay */
    nrf24l01_auto_retransmit_delay_750us,       /**< 750 µs delay */
    nrf24l01_auto_retransmit_delay_1000us,      /**< 1000 µs delay */
    nrf24l01_auto_retransmit_delay_1250us,      /**< 1250 µs delay */
    nrf24l01_auto_retransmit_delay_1500us,      /**< 1500 µs delay */
    nrf24l01_auto_retransmit_delay_1750us,      /**< 1750 µs delay */
    nrf24l01_auto_retransmit_delay_2000us,      /**< 2000 µs delay */
    nrf24l01_auto_retransmit_delay_2250us,      /**< 2250 µs delay */
    nrf24l01_auto_retransmit_delay_2500us,      /**< 2500 µs delay */
    nrf24l01_auto_retransmit_delay_2750us,      /**< 2750 µs delay */
    nrf24l01_auto_retransmit_delay_3000us,      /**< 3000 µs delay */
    nrf24l01_auto_retransmit_delay_3250us,      /**< 3250 µs delay */
    nrf24l01_auto_retransmit_delay_3500us,      /**< 3500 µs delay */
    nrf24l01_auto_retransmit_delay_3750us,      /**< 3750 µs delay */
    nrf24l01_auto_retransmit_delay_4000us,      /**< 4000 µs delay */
} nrf24l01_auto_retransmit_delay;

/**
 * @brief Auto retransmit count settings
 */
typedef enum{
    nrf24l01_auto_retransmit_count_disable = 0,  /**< Disable auto retransmit */
    nrf24l01_auto_retransmit_count_1,             /**< Up to 1 retransmit */
    nrf24l01_auto_retransmit_count_2,             /**< Up to 2 retransmits */
    nrf24l01_auto_retransmit_count_3,             /**< Up to 3 retransmits */
    nrf24l01_auto_retransmit_count_4,             /**< Up to 4 retransmits */
    nrf24l01_auto_retransmit_count_5,             /**< Up to 5 retransmits */
    nrf24l01_auto_retransmit_count_6,             /**< Up to 6 retransmits */
    nrf24l01_auto_retransmit_count_7,             /**< Up to 7 retransmits */
    nrf24l01_auto_retransmit_count_8,             /**< Up to 8 retransmits */
    nrf24l01_auto_retransmit_count_9,             /**< Up to 9 retransmits */
    nrf24l01_auto_retransmit_count_10,            /**< Up to 10 retransmits */
    nrf24l01_auto_retransmit_count_11,            /**< Up to 11 retransmits */
    nrf24l01_auto_retransmit_count_12,            /**< Up to 12 retransmits */
    nrf24l01_auto_retransmit_count_13,            /**< Up to 13 retransmits */
    nrf24l01_auto_retransmit_count_14,            /**< Up to 14 retransmits */
    nrf24l01_auto_retransmit_count_15,            /**< Up to 15 retransmits */
} nrf24l01_auto_retransmit_count;

/**
 * @brief Interrupt types
 */
typedef enum{
    nrf24l01_irq_rx_data_ready = 0,      /**< RX Data Ready interrupt */
    nrf24l01_irq_tx_data_sent,           /**< TX Data Sent interrupt */
    nrf24l01_maximum_retransmitted,      /**< Maximum retransmitted interrupt */
} nrf24l01_irq;

/** @} */ // End of NRF24L01_ENUMS group

/**
 * @defgroup NRF24L01_STRUCTS Structure Definitions
 * @brief Structure types for device and data pipe configuration
 * @{
 */

/**
 * @brief Data pipe configuration structure
 */
typedef struct{
    uint8_t nrf24l01_data_pipe_enable;                    /**< Enable/disable data pipe */
    uint8_t nrf24l01_data_pipe_dyn_payload_length_enable; /**< Enable dynamic payload length */
    uint8_t nrf24l01_data_pipe_auto_ack;                  /**< Enable auto acknowledgment */
    uint8_t* nrf24l01_data_pipe_receive_address;          /**< Receive address for this pipe */
    uint8_t nrf24l01_data_pipe_payload_width;             /**< Static payload width (if dynamic disabled) */
    uint8_t* nrf24l01_data_pipe_payload;                  /**< Payload data buffer */
} nrf24l01_data_pipe;

/**
 * @brief Main nRF24L01 device configuration structure
 */
typedef struct{
    SPI_HandleTypeDef * spi;                         /**< SPI handle for communication */
    GPIO_TypeDef *irq_port, *ce_port, *csn_port;    /**< GPIO ports for control pins */
    uint8_t irq_pin, ce_pin, csn_pin;               /**< GPIO pin numbers */
    TIM_HandleTypeDef * timer;                       /**< Timer handle for microsecond delays */
    uint8_t power_up;                                /**< Power state (0=down, 1=up) */
    uint8_t primary_rx;                              /**< Primary mode (0=TX, 1=RX) */
    nrf24l01_address_width address_width;            /**< Address width configuration */
    nrf24l01_auto_retransmit_delay auto_retransmit_delay; /**< Auto retransmit delay */
    nrf24l01_auto_retransmit_count auto_retransmit_count; /**< Auto retransmit count */
    uint8_t frequency_channel;                       /**< RF frequency channel (0-125) */
    nrf24l01_air_data_rate air_data_rate;           /**< Air data rate setting */
    nrf24l01_rf_output_power rf_output_power;        /**< RF output power level */
    uint8_t setup_lna_gain;                          /**< LNA gain setting */
    nrf24l01_data_pipe data_pipe[6];                 /**< Configuration for all 6 data pipes */
    uint8_t* transmit_address;                       /**< Transmit address */
    uint8_t dynamic_payload_length_enable;           /**< Global dynamic payload enable */
    uint8_t payload_with_ack_enable;                 /**< Payload with ACK enable */
    uint8_t dynamic_ack_enable;                      /**< Dynamic ACK enable */
} nrf24l01_device;

/** @} */ // End of NRF24L01_STRUCTS group

/**
 * @defgroup NRF24L01_FUNCTIONS Function Declarations
 * @brief Public API function declarations
 * @{
 */

/**
 * @defgroup NRF24L01_INIT Initialization Functions
 * @brief Functions for device initialization and configuration
 * @{
 */

/**
 * @brief Get default device configuration
 * @return nrf24l01_device Default configuration structure
 *
 * Returns a device structure with sensible default values:
 * - Power down mode
 * - 5-byte address width
 * - Channel 2 (2.402 GHz)
 * - 1 Mbps data rate
 * - 0 dBm output power
 * - Auto retransmit: 250µs delay, 3 retries
 */
nrf24l01_device nrf24l01_get_default_config();

/**
 * @brief Initialize nRF24L01 device
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 *
 * Initializes the nRF24L01 module with the provided configuration.
 * This function configures all registers according to the device structure
 * and verifies successful communication with the module.
 *
 * @note Make sure to configure SPI handle and GPIO pins before calling this function
 */
uint8_t nrf24l01_init(nrf24l01_device * device);

/**
 * @brief Deinitialize nRF24L01 device
 * @param device Pointer to device configuration structure
 *
 * Powers down the device and resets control pins to safe states.
 */
void nrf24l01_deinit(nrf24l01_device * device);

/**
 * @brief Initialize specific data pipe
 * @param device Pointer to device configuration structure
 * @param pipe_number Data pipe number (0-5)
 * @return 0 on success, non-zero on error
 *
 * Configures a specific data pipe according to the settings in the
 * device structure's data_pipe array.
 */
uint8_t nrf24l01_init_data_pipe(nrf24l01_device * device, uint8_t pipe_number);

/** @} */ // End of NRF24L01_INIT group

/**
 * @defgroup NRF24L01_LOW_LEVEL Low Level Functions
 * @brief Hardware abstraction and low-level communication functions
 * @{
 */

/**
 * @brief Generate microsecond delay
 * @param device Pointer to device configuration structure
 * @param us Delay in microseconds
 *
 * Uses the configured timer to generate precise microsecond delays.
 */
void nrf24l01_delay(nrf24l01_device * device, uint16_t us);

/**
 * @brief Select nRF24L01 chip (CSN low)
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 */
uint8_t nrf24l01_chip_select(nrf24l01_device * device);

/**
 * @brief Deselect nRF24L01 chip (CSN high)
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 */
uint8_t nrf24l01_chip_deselect(nrf24l01_device * device);

/**
 * @brief Enable nRF24L01 chip (CE high)
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 */
uint8_t nrf24l01_chip_enable(nrf24l01_device * device);

/**
 * @brief Disable nRF24L01 chip (CE low)
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 */
uint8_t nrf24l01_chip_disable(nrf24l01_device * device);

/**
 * @brief Send single byte command
 * @param device Pointer to device configuration structure
 * @param command Command byte to send
 * @return Status register value
 */
uint8_t nrf24l01_send_command(nrf24l01_device* device, uint8_t command);

/** @} */ // End of NRF24L01_LOW_LEVEL group

/**
 * @defgroup NRF24L01_REGISTER_ACCESS Register Access Functions
 * @brief Functions for reading and writing registers
 * @{
 */

/**
 * @brief Read from nRF24L01 register
 * @param device Pointer to device configuration structure
 * @param reg Register address to read from
 * @param data Pointer to buffer for received data
 * @param length Number of bytes to read
 * @return Status register value
 */
uint8_t nrf24l01_read_register(nrf24l01_device * device, uint8_t reg, uint8_t* data, uint16_t length);

/**
 * @brief Write to nRF24L01 register
 * @param device Pointer to device configuration structure
 * @param reg Register address to write to
 * @param data Pointer to data buffer to write
 * @param length Number of bytes to write
 * @return Status register value
 */
uint8_t nrf24l01_write_register(nrf24l01_device * device, uint8_t reg, uint8_t* data, uint16_t length);

/** @} */ // End of NRF24L01_REGISTER_ACCESS group

/**
 * @defgroup NRF24L01_PAYLOAD_ACCESS Payload Access Functions
 * @brief Functions for reading and writing payload data
 * @{
 */

/**
 * @brief Read RX payload from FIFO
 * @param device Pointer to device configuration structure
 * @param data Pointer to buffer for received payload
 * @param length Number of bytes to read
 * @return Status register value
 */
uint8_t nrf24l01_read_rx_payload(nrf24l01_device * device, uint8_t* data, uint16_t length);

/**
 * @brief Write TX payload to FIFO
 * @param device Pointer to device configuration structure
 * @param data Pointer to payload data to transmit
 * @param length Number of bytes to write
 * @return Status register value
 */
uint8_t nrf24l01_write_tx_payload(nrf24l01_device * device, uint8_t* data , uint16_t length);

/**
 * @brief Write ACK payload for specific pipe
 * @param device Pointer to device configuration structure
 * @param data Pointer to ACK payload data
 * @param pipe Data pipe number (0-5)
 * @param length Number of bytes to write
 * @return Status register value
 *
 * Writes payload data that will be transmitted along with ACK packet
 * for the specified data pipe.
 */
uint8_t nrf24l01_write_ack_payload(nrf24l01_device * device, uint8_t* data, uint8_t pipe, uint16_t length);

/**
 * @brief Write TX payload without expecting ACK
 * @param device Pointer to device configuration structure
 * @param data Pointer to payload data to transmit
 * @param length Number of bytes to write
 * @return Status register value
 *
 * Writes payload data that will be transmitted without requesting
 * an acknowledgment from the receiver.
 */
uint8_t nrf24l01_write_tx_payload_no_ack(nrf24l01_device * device, uint8_t * data, uint16_t length);

/**
 * @brief Read top RX payload width
 * @param device Pointer to device configuration structure
 * @param data Pointer to store payload width value
 * @return Status register value
 *
 * Reads the payload width of the top RX payload in the RX FIFO.
 * Used with dynamic payload length feature.
 */
uint8_t nrf24l01_read_rx_payload_width(nrf24l01_device * device, uint8_t* data);

/** @} */ // End of NRF24L01_PAYLOAD_ACCESS group

/**
 * @defgroup NRF24L01_FIFO_CONTROL FIFO Control Functions
 * @brief Functions for managing TX and RX FIFOs
 * @{
 */

/**
 * @brief Flush TX FIFO
 * @param device Pointer to device configuration structure
 * @return Status register value
 *
 * Clears all pending payloads in the TX FIFO. Used in TX mode.
 */
uint8_t nrf24l01_flush_tx(nrf24l01_device * device);

/**
 * @brief Flush RX FIFO
 * @param device Pointer to device configuration structure
 * @return Status register value
 *
 * Clears all received payloads in the RX FIFO. Used in RX mode.
 */
uint8_t nrf24l01_flush_rx(nrf24l01_device * device);

/**
 * @brief Reuse last transmitted payload
 * @param device Pointer to device configuration structure
 * @return Status register value
 *
 * Reuses the last transmitted payload. The packet will be retransmitted
 * as long as CE is held high.
 */
uint8_t nrf24l01_reuse_tx_payload(nrf24l01_device * device);

/** @} */ // End of NRF24L01_FIFO_CONTROL group

/**
 * @defgroup NRF24L01_ADVANCED_FEATURES Advanced Feature Functions
 * @brief Functions for advanced nRF24L01 features
 * @{
 */

/**
 * @brief Activate extra features
 * @param device Pointer to device configuration structure
 * @return Status register value
 *
 * Activates advanced features like dynamic payload length,
 * payload with ACK, and dynamic ACK features.
 */
uint8_t nrf24l01_activate_extra_features(nrf24l01_device * device);

/**
 * @brief No operation command
 * @param device Pointer to device configuration structure
 * @return Status register value
 *
 * Sends NOP command, useful for reading status register without
 * performing any other operation.
 */
uint8_t nrf24l01_nop(nrf24l01_device * device);

/** @} */ // End of NRF24L01_ADVANCED_FEATURES group

/**
 * @defgroup NRF24L01_POWER_CONTROL Power Control Functions
 * @brief Functions for managing device power state
 * @{
 */

/**
 * @brief Power up the nRF24L01 device
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 *
 * Powers up the device and waits for power-up settling time.
 * Device must be powered up before any RF operation.
 */
uint8_t nrf24l01_power_up(nrf24l01_device * device);

/**
 * @brief Power down the nRF24L01 device
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 *
 * Powers down the device to minimize current consumption.
 * All register contents are maintained during power down.
 */
uint8_t nrf24l01_power_down(nrf24l01_device * device);

/** @} */ // End of NRF24L01_POWER_CONTROL group

/**
 * @defgroup NRF24L01_MODE_CONTROL Mode Control Functions
 * @brief Functions for switching between TX and RX modes
 * @{
 */

/**
 * @brief Configure device for transmit mode
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 *
 * Configures the device as a primary transmitter (PTX).
 * Device will be ready to transmit packets.
 */
uint8_t nrf24l01_transmit(nrf24l01_device * device);

/**
 * @brief Configure device for receive mode
 * @param device Pointer to device configuration structure
 * @return 0 on success, non-zero on error
 *
 * Configures the device as a primary receiver (PRX).
 * Device will listen for incoming packets on enabled data pipes.
 */
uint8_t nrf24l01_listen(nrf24l01_device * device);

/** @} */ // End of NRF24L01_MODE_CONTROL group

/**
 * @defgroup NRF24L01_FEATURE_CONTROL Feature Control Functions
 * @brief Functions for enabling/disabling advanced features
 * @{
 */

/**
 * @brief Enable/disable dynamic payload length
 * @param device Pointer to device configuration structure
 * @param enable 1 to enable, 0 to disable
 * @return 0 on success, non-zero on error
 *
 * When enabled, payload length is determined dynamically based on
 * the received packet rather than using fixed width.
 */
uint8_t nrf24l01_dynamic_payload_length(nrf24l01_device * device, uint8_t enable);

/**
 * @brief Enable/disable payload with ACK
 * @param device Pointer to device configuration structure
 * @param enable 1 to enable, 0 to disable
 * @return 0 on success, non-zero on error
 *
 * When enabled, allows sending payload data along with ACK packets.
 */
uint8_t nrf24l01_payload_with_ack(nrf24l01_device * device, uint8_t enable);

/**
 * @brief Enable/disable dynamic ACK
 * @param device Pointer to device configuration structure
 * @param enable 1 to enable, 0 to disable
 * @return 0 on success, non-zero on error
 *
 * When enabled, allows selective disable of auto-acknowledgment
 * for individual packets using W_TX_PAYLOAD_NOACK command.
 */
uint8_t nrf24l01_dynamic_ack(nrf24l01_device * device, uint8_t enable);

/** @} */ // End of NRF24L01_FEATURE_CONTROL group

/**
 * @defgroup NRF24L01_PIPE_CONTROL Data Pipe Control Functions
 * @brief Functions for managing individual data pipes
 * @{
 */

/**
 * @brief Enable/disable dynamic payload length for specific pipe
 * @param device Pointer to device configuration structure
 * @param pipe_number Data pipe number (0-5)
 * @param enable 1 to enable, 0 to disable
 * @return 0 on success, non-zero on error
 *
 * Enables or disables dynamic payload length for the specified data pipe.
 * Global dynamic payload length must also be enabled.
 */
uint8_t nrf24l01_data_pipe_dynamic_payload_length(nrf24l01_device * device, uint8_t pipe_number, uint8_t enable);

/**
 * @brief Enable/disable specific data pipe
 * @param device Pointer to device configuration structure
 * @param pipe_number Data pipe number (0-5)
 * @param enable 1 to enable, 0 to disable
 * @return 0 on success, non-zero on error
 *
 * Enables or disables reception on the specified data pipe.
 */
uint8_t nrf24l01_data_pipe_enable(nrf24l01_device * device, uint8_t pipe_number, uint8_t enable);

/**
 * @brief Enable/disable auto-acknowledgment for specific pipe
 * @param device Pointer to device configuration structure
 * @param pipe_number Data pipe number (0-5)
 * @param enable 1 to enable, 0 to disable
 * @return 0 on success, non-zero on error
 *
 * Enables or disables automatic acknowledgment for the specified data pipe.
 */
uint8_t nrf24l01_data_pipe_auto_ack(nrf24l01_device * device, uint8_t pipe_number, uint8_t enable);

/**
 * @brief Set receive address for specific data pipe
 * @param device Pointer to device configuration structure
 * @param pipe_number Data pipe number (0-5)
 * @param address Pointer to address bytes
 * @param length Address length in bytes (3-5)
 * @return 0 on success, non-zero on error
 *
 * Sets the receive address for the specified data pipe.
 *
 * @note Pipes 0 and 1 use full address length, pipes 2-5 use only LSB
 * (the remaining bytes are shared with pipe 1).
 */
uint8_t nrf24l01_data_pipe_address(nrf24l01_device * device, uint8_t pipe_number, uint8_t * address, uint8_t length);

/**
 * @brief Set payload width for specific data pipe
 * @param device Pointer to device configuration structure
 * @param pipe_number Data pipe number (0-5)
 * @param width Payload width in bytes (1-32)
 * @return 0 on success, non-zero on error
 *
 * Sets the expected payload width for the specified data pipe.
 * Only used when dynamic payload length is disabled.
 *
 * @note Setting width to 0 disables the pipe
 */
uint8_t nrf24l01_data_pipe_payload_width(nrf24l01_device * device, uint8_t pipe_number, uint8_t width);

/** @} */ // End of NRF24L01_PIPE_CONTROL group

/**
 * @defgroup NRF24L01_INTERRUPT_CONTROL Interrupt Control Functions
 * @brief Functions for managing interrupt sources
 * @{
 */

/**
 * @brief Enable/disable specific interrupt
 * @param device Pointer to device configuration structure
 * @param irq Interrupt type to control
 * @param enable 1 to enable, 0 to disable (mask)
 * @return 0 on success, non-zero on error
 *
 * Controls interrupt masking for the specified interrupt source.
 * When enabled (0), the interrupt will trigger the IRQ pin.
 * When disabled (1), the interrupt is masked.
 *
 * @note Interrupt enable logic is inverted - 0 enables, 1 disables
 */
uint8_t nrf24l01_interrupt(nrf24l01_device * device, nrf24l01_irq irq, uint8_t enable);

/**
 * @brief Clear specific interrupt flag
 * @param device Pointer to device configuration structure
 * @param irq Interrupt type to clear
 * @return 0 on success, non-zero on error
 *
 * Clears the specified interrupt flag in the status register.
 * This must be done to reset the IRQ pin after an interrupt occurs.
 */
uint8_t nrf24l01_clear_interrupt_flag(nrf24l01_device * device, nrf24l01_irq irq);

/** @} */ // End of NRF24L01_INTERRUPT_CONTROL group

/** @} */ // End of NRF24L01_FUNCTIONS group

#endif //NRF24L01_DRIVER_NRF24L01_H

/**
 * @mainpage nRF24L01 Driver Documentation
 *
 * @section intro_sec Introduction
 *
 * This is a comprehensive driver library for the nRF24L01 2.4GHz wireless transceiver module,
 * designed specifically for STM32 microcontrollers using the HAL library.
 *
 * @section features_sec Key Features
 *
 * - Full register-level control of nRF24L01 functionality
 * - Support for all 6 data pipes with individual configuration
 * - Auto-acknowledgment and auto-retransmission
 * - Dynamic payload lengths
 * - Payload with ACK capability
 * - Configurable RF power levels and data rates
 * - Comprehensive interrupt handling
 * - Easy-to-use high-level API functions
 *
 * @section usage_sec Basic Usage
 *
 * @subsection init_subsec Initialization
 * @code
 * #include "nrf24l01.h"
 *
 * // Get default configuration
 * nrf24l01_device nrf = nrf24l01_get_default_config();
 *
 * // Configure hardware connections
 * nrf.spi = &hspi1;          // SPI handle
 * nrf.ce_port = GPIOA;       // CE pin port
 * nrf.ce_pin = 8;            // CE pin number
 * nrf.csn_port = GPIOA;      // CSN pin port
 * nrf.csn_pin = 9;           // CSN pin number
 * nrf.irq_port = GPIOA;      // IRQ pin port (optional)
 * nrf.irq_pin = 10;          // IRQ pin number (optional)
 * nrf.timer = &htim2;        // Timer for microsecond delays
 *
 * // Initialize the device
 * if (nrf24l01_init(&nrf) == 0) {
 *     // Initialization successful
 * }
 * @endcode
 *
 * @subsection tx_subsec Transmitter Setup
 * @code
 * // Configure transmit address
 * uint8_t tx_address[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
 * nrf.transmit_address = tx_address;
 * nrf24l01_write_register(&nrf, TX_ADDR, nrf.transmit_address, 5);
 *
 * // Configure data pipe 0 for auto-ack (must match TX address)
 * nrf24l01_data_pipe_address(&nrf, 0, nrf.transmit_address, 5);
 * nrf24l01_data_pipe_enable(&nrf, 0, 1);
 * nrf24l01_data_pipe_auto_ack(&nrf, 0, 1);
 * nrf24l01_data_pipe_payload_width(&nrf, 0, 32);
 *
 * // Send data
 * uint8_t data[] = "Hello nRF24L01!";
 * nrf24l01_write_tx_payload(&nrf, data, sizeof(data)); // write payload to TX FIFO
 * nrf24l01_transmit(&nrf); // put device in TX Mode
 * @endcode
 *
 * @subsection rx_subsec Receiver Setup
 * @code
 * // Configure receive address for pipe 1
 * uint8_t rx_address[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
 * nrf24l01_data_pipe_address(&nrf, 1, rx_address, 5);
 * nrf24l01_data_pipe_enable(&nrf, 1, 1);
 * nrf24l01_data_pipe_auto_ack(&nrf, 1, 1);
 * nrf24l01_data_pipe_payload_width(&nrf, 1, 32);
 *
 * // make the device PRX
 * uint8_t config_register = 0;
 * nrf24l01_read_register(&nrf, CONFIG, &config_register, 1);
 * config_register |= PRX;
 * nrf24l01_wrtie_register(&nrf, CONFIG, &config_register, 1);
 *
 * // Listen to new available packets
 * nrf24l01_listen(&nrf);
 *
 * // Check for received data
 * uint8_t status = nrf24l01_nop(&nrf);
 * do {
 * if (status & RX_DR) {
 *     uint8_t received_data[32];
 *     nrf24l01_read_rx_payload(&nrf, received_data, 32);
 *     nrf24l01_clear_interrupt_flag(&nrf, nrf24l01_irq_rx_data_ready);
 *     break;
 * }
 * status = nrf24l01_nop(&nrf);
 * }while(1);
 *
 * @endcode
 *
 * @section config_sec Configuration Options
 *
 * The driver supports extensive configuration options:
 *
 * - **RF Channel**: 0-125 (2.400-2.525 GHz)
 * - **Data Rate**: 1 Mbps or 2 Mbps
 * - **Output Power**: -18, -12, -6, or 0 dBm
 * - **Address Width**: 3, 4, or 5 bytes
 * - **Auto Retransmit**: Configurable delay (250µs-4ms) and count (0-15)
 * - **Payload Length**: Fixed (1-32 bytes) or dynamic
 *
 * @section advanced_sec Advanced Features
 *
 * @subsection dpl_subsec Dynamic Payload Length
 * @code
 * // Enable dynamic payload length globally
 * nrf24l01_dynamic_payload_length(&nrf, 1);
 *
 * // Enable for specific pipes
 * nrf24l01_data_pipe_dynamic_payload_length(&nrf, 0, 1);
 * nrf24l01_data_pipe_dynamic_payload_length(&nrf, 1, 1);
 * @endcode
 *
 * @subsection ack_payload_subsec ACK Payloads
 * @code
 * // Enable payload with ACK feature
 * nrf24l01_payload_with_ack(&nrf, 1);
 *
 * // Write ACK payload for pipe 1
 * uint8_t ack_data[] = "ACK";
 * nrf24l01_write_ack_payload(&nrf, ack_data, 1, sizeof(ack_data));
 * @endcode
 *
 * @section troubleshooting_sec Troubleshooting
 *
 * Common issues and solutions:
 *
 * - **No communication**: Check SPI connections and timing
 * - **Transmission fails**: Verify addresses match between TX and RX
 * - **High packet loss**: Adjust RF power level or channel
 * - **Interrupt not working**: Ensure IRQ pin is configured and interrupt flags are cleared
 *
 * @section license_sec License
 *
 * This driver is provided as-is for educational and commercial use.
 *
 * @author Trần Minh Khải
 * @date September 18, 2025
 * @version 1.0
 */