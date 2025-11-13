/*
 * LED_GPIO_Expander.h
 *
 *  Created on: Nov 13, 2025
 *      Author: hugoc
 */

#ifndef LED_GPIO_EXPANDER_H_
#define LED_GPIO_EXPANDER_H_


#include "stm32l4xx_hal.h"

// MCP23S17 Definitions
#define MCP23S17_ADDR 0x00 // Assuming hardware address 000 (A2=A1=A0=0)
#define MCP23S17_WRITE (0x40 | (MCP23S17_ADDR << 1)) // Opcode for write
#define MCP23S17_READ (0x41 | (MCP23S17_ADDR << 1)) // Opcode for read

// Registers (BANK=0 mode, default)
#define MCP23S17_IODIRA 0x00
#define MCP23S17_IODIRB 0x01
#define MCP23S17_GPIOA 0x12
#define MCP23S17_GPIOB 0x13
#define MCP23S17_OLATA 0x14 // Optional latch registers
#define MCP23S17_OLATB 0x15
#define MCP23S17_IOCONA 0x0A // Configuration register

// SPI Handle (assuming defined in main or elsewhere)
extern SPI_HandleTypeDef hspi3;

// GPIO Pins
#define MCP_CS_GPIO_PORT GPIOB
#define MCP_CS_PIN GPIO_PIN_7
#define MCP_RESET_GPIO_PORT GPIOA
#define MCP_RESET_PIN GPIO_PIN_0

// Function Prototypes
void MCP23S17_Init(void);
void MCP23S17_WriteRegister(uint8_t reg, uint8_t value);
uint8_t MCP23S17_ReadRegister(uint8_t reg);
void MCP23S17_SetAllPinsHigh(void);

#endif /* LED_GPIO_EXPANDER_H_ */
