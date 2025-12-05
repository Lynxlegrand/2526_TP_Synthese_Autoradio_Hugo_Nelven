/*
 * chenille.c
 *
 *  Created on: 21 nov. 2025
 *      Author: hugoc
 */

#include "chenille.h"
#include "main.h"
#include "spi.h"
// ===================================================================
// Instance globale initialisée
// ===================================================================
MCP23S17_HandleTypeDef hmcp23s17 = {
    .hspi       = &hspi3,
    .cs_port    = GPIOB,
    .cs_pin     = GPIO_PIN_7,
    .reset_port = GPIOA,
    .reset_pin  = GPIO_PIN_0,
    .address    = MCP23S17_ADDR
};

// ===================================================================
// Fonctions
// ===================================================================

void MCP23S17_Init(void) {
    // CS high
    HAL_GPIO_WritePin(hmcp23s17.cs_port, hmcp23s17.cs_pin, GPIO_PIN_SET);

    // Reset pulse
    HAL_GPIO_WritePin(hmcp23s17.reset_port, hmcp23s17.reset_pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(hmcp23s17.reset_port, hmcp23s17.reset_pin, GPIO_PIN_SET);
    HAL_Delay(1);

    // Config IOCON : BANK=0, SEQOP=0
    MCP23S17_WriteRegister(MCP23S17_IOCON, 0x20);

    // Tous les pins en sortie (0 = output)
    MCP23S17_WriteRegister(MCP23S17_IODIRA, 0x00);
    MCP23S17_WriteRegister(MCP23S17_IODIRB, 0x00);

    // Éteindre toutes les LEDs au démarrage
    MCP23S17_WriteRegister(MCP23S17_GPIOA, 0x00);
    MCP23S17_WriteRegister(MCP23S17_GPIOB, 0x00);
}

void MCP23S17_WriteRegister(uint8_t reg, uint8_t value) {
    uint8_t txData[3] = {
        (uint8_t)(0x40 | (hmcp23s17.address << 1)),  // Write opcode
        reg,
        value
    };

    HAL_GPIO_WritePin(hmcp23s17.cs_port, hmcp23s17.cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hmcp23s17.hspi, txData, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hmcp23s17.cs_port, hmcp23s17.cs_pin, GPIO_PIN_SET);
}

uint8_t MCP23S17_ReadRegister(uint8_t reg) {
    uint8_t txData[3] = {
        (uint8_t)(0x41 | (hmcp23s17.address << 1)),  // Read opcode
        reg,
        0x00
    };
    uint8_t rxData[3] = {0};

    HAL_GPIO_WritePin(hmcp23s17.cs_port, hmcp23s17.cs_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(hmcp23s17.hspi, txData, rxData, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hmcp23s17.cs_port, hmcp23s17.cs_pin, GPIO_PIN_SET);

    return rxData[2];
}

// Allume TOUTES les LEDs
void MCP23S17_SetAllPinsHigh(void) {
    MCP23S17_WriteRegister(MCP23S17_GPIOA, 0xFF);
    MCP23S17_WriteRegister(MCP23S17_GPIOB, 0xFF);
}

// Éteint toutes les LEDs
void MCP23S17_SetAllPinsLow(void) {
    MCP23S17_WriteRegister(MCP23S17_GPIOA, 0x00);
    MCP23S17_WriteRegister(MCP23S17_GPIOB, 0x00);
}

// Allume une LED spécifique (0 à 15)
void Select_LED(char port, uint8_t led,uint8_t state)
{
    if (led > 7) return;

    uint8_t reg = (port == 'A' || port == 'a') ? MCP23S17_GPIOA : MCP23S17_GPIOB;
    uint8_t current = MCP23S17_ReadRegister(reg);
    // On part du principe que tout est allumé (0xFF)
    // On force un seul bit à 0 → éteint la LED
    if (state == 1)
    {
        uint8_t pattern = 0xFF;           // 11111111
        current &= ~(1 << led);           // Met le bit `led` à 0
    }

    if (state == 0)
	{
		uint8_t pattern = 0xFF;           // 11111111
		current |= (1 << led);           // Met le bit `led` à 0
	}


    MCP23S17_WriteRegister(reg, current);
}
// Éteint une LED spécifique
void Clear_LED(char port, uint8_t led) {
    if (led > 7) return;

    uint8_t reg = (port == 'A') ? MCP23S17_GPIOA : MCP23S17_GPIOB;
    uint8_t current = MCP23S17_ReadRegister(reg);
    current &= ~(1 << led);  // Éteint la LED
    MCP23S17_WriteRegister(reg, current);
}


