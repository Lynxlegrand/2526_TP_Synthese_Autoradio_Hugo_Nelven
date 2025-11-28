/*
 * sgtl5000.c
 *
 *  Created on: Nov 28, 2025
 *      Author: nelven
 */


#include "sgtl5000.h"

#include "sgtl5000.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c2;

/**
 * @brief  Écrit un registre 16 bits dans le SGTL5000
 */
static HAL_StatusTypeDef CODEC_Write(uint16_t reg, uint16_t value)
{
    uint8_t data[2];
    data[0] = (value >> 8) & 0xFF;   // MSB
    data[1] = value & 0xFF;          // LSB

    return HAL_I2C_Mem_Write(&hi2c2,
                             (uint16_t)CODEC_I2C_ADDR,
                             reg,
                             I2C_MEMADD_SIZE_16BIT,
                             data,
                             2,
                             HAL_MAX_DELAY);
}

/**
 * @brief  Initialisation complète du SGTL5000
 */
void Init_sgtl5000(void)
{
    // LINREG CTRL
    CODEC_Write(REG_LINREG_CTRL_ADDR, REG_LINREG_CTRL_VALUE);

    // ANA POWER
    CODEC_Write(REG_ANA_POWER_ADDR, REG_ANA_POWER_VALUE);

    vTaskDelay(pdMS_TO_TICKS(100));

    // DIG POWER
    CODEC_Write(REG_DIG_POWER_ADDR, REG_DIG_POWER_VALUE);

    // CLK CTRL
    CODEC_Write(REG_CLK_CTRL_ADDR, REG_CLK_CTRL_VALUE);

    // I2S CTRL
    CODEC_Write(REG_I2S_CTRL_ADDR, REG_I2S_CTRL_VALUE);

    // SSS CTRL
    CODEC_Write(REG_SSS_CTRL_ADDR, REG_SSS_CTRL_VALUE);

    // ADCDAC CTRL
    CODEC_Write(REG_ADCDAC_CTRL_ADDR, REG_ADCDAC_CTRL_VALUE);

    // DAC Volume
    CODEC_Write(REG_DAC_VOL_ADDR, REG_DAC_VOL_VALUE);
}
