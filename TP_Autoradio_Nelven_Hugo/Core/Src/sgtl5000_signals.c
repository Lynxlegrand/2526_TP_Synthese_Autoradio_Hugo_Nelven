/*
 * sgtl5000_signals.c
 *
 *  Created on: Dec 5, 2025
 *      Author: hugoc
 */
#include "sgtl5000_signals.h"



void sgtl5000_fill_triangle(h_sgtl5000_t *h_sgtl5000, int16_t amplitude)
{
    if (!h_sgtl5000 || amplitude <= 0) return;

    const int channels = AUDIO_NUM_CHANNELS;       // 2 canaux
    const int frames = AUDIO_BUFFER_LENGTH;        // taille du buffer DMA utilisé à chaque fois

    // Calcul du nombre d’échantillons par période
    int period = frames;   // ici on veut 1 période par buffer
    if (period < 2) period = 2;

    for (int frame = 0; frame < frames; ++frame)
    {
        int pos = frame % period;
        int32_t sample = 0;

        int half = period / 2;
        if (pos < half)
        {
            sample = -amplitude + (2 * amplitude * pos) / half;
        }
        else
        {
            int pos2 = pos - half;
            int half2 = period - half;
            sample = amplitude - (2 * amplitude * pos2) / half2;
        }

        // Interleave LR LR ...
        int base_index = frame * channels;
        for (int ch = 0; ch < channels; ++ch)
        {
            h_sgtl5000->sai_tx_buffer[base_index + ch] = (int16_t)sample;
        }
    }
}


void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
    if (hsai != sgtl5000.hsai_rx) return;

    int16_t *src = sgtl5000.sai_rx_buffer;
    int16_t *dst = sgtl5000.sai_tx_buffer;

    size_t samples = (size_t)AUDIO_BUFFER_LENGTH * (size_t)AUDIO_NUM_CHANNELS * (size_t) AUDIO_DOUBLE_BUFFER;

    memcpy(dst, src, samples * sizeof(int16_t));

    // --- VU meter sur 1ère moitié ---
    VU_Update(src, samples);

    HAL_GPIO_TogglePin(GPIOA, LD2_Pin);
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
    if (hsai != sgtl5000.hsai_rx) return;

    int16_t *src = sgtl5000.sai_rx_buffer;
    int16_t *dst = sgtl5000.sai_tx_buffer;

    size_t samples = (size_t)AUDIO_BUFFER_LENGTH * (size_t)AUDIO_NUM_CHANNELS * (size_t) AUDIO_DOUBLE_BUFFER;

    memcpy(dst, src, samples * sizeof(int16_t));

    // --- VU meter sur 2ème moitié ---
    VU_Update(src + samples/2, samples/2);

    HAL_GPIO_TogglePin(GPIOA, LD2_Pin);
}
