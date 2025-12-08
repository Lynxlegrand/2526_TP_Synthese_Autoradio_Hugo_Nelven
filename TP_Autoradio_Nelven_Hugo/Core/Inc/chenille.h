/*
 * chenille.h
 *
 *  Created on: 21 nov. 2025
 *      Author: hugoc
 */

#ifndef INC_CHENILLE_H_
#define INC_CHENILLE_H_

#include "main.h"
#include "spi.h"

// ===================================================================
// MCP23S17 Definitions
// ===================================================================
#define MCP23S17_ADDR       0x00
#define MCP23S17_WRITE      (0x40 | (MCP23S17_ADDR << 1))  // 0x40
#define MCP23S17_READ       (0x41 | (MCP23S17_ADDR << 1))  // 0x41

#define MCP23S17_IODIRA     0x00
#define MCP23S17_IODIRB     0x01
#define MCP23S17_GPIOA      0x12
#define MCP23S17_GPIOB      0x13
#define MCP23S17_OLATA      0x14
#define MCP23S17_OLATB      0x15
#define MCP23S17_IOCON      0x0A

void MCP23S17_Init(void);
void MCP23S17_WriteRegister(uint8_t reg, uint8_t value);
uint8_t MCP23S17_ReadRegister(uint8_t reg);
void MCP23S17_SetAllPinsHigh(void);
void MCP23S17_SetAllPinsLow(void);
void Select_LED(char port, uint8_t led,uint8_t state);
void Clear_LED(char port, uint8_t led);

// ===================================================================
// Structure
// ===================================================================
typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef      *cs_port;
    uint16_t           cs_pin;
    GPIO_TypeDef      *reset_port;
    uint16_t           reset_pin;
    uint8_t            address;
} MCP23S17_HandleTypeDef;

// ===================================================================
// Instance globale
// ===================================================================
extern MCP23S17_HandleTypeDef hmcp23s17;

// ===================================================================
// Fonctions publiques
// ===================================================================
// Convertit amplitude (int16) vers 0..8 LEDs
void VU_Update(int16_t *buffer, size_t samples);

#endif /* INC_CHENILLE_H_ */
