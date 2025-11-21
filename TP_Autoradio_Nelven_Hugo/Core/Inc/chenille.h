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


#endif /* INC_CHENILLE_H_ */
