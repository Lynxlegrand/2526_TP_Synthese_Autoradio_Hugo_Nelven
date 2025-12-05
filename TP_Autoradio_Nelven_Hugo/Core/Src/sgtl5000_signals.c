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



//void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
//{
//    if(hsai == sgtl5000.hsai_rx)
//    {
//        // Copier première moitié du buffer
//        int half = (AUDIO_BUFFER_LENGTH * AUDIO_NUM_CHANNELS * AUDIO_DOUBLE_BUFFER) / 2;
//        for(int i = 0; i < half; i++)
//        {
//            sgtl5000.sai_tx_buffer[i] = sgtl5000.sai_rx_buffer[i];
//        }
//    }
//}
//
//void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
//{
//    if(hsai == sgtl5000.hsai_rx)
//    {
//        // Copier seconde moitié du buffer
//        int half = (AUDIO_BUFFER_LENGTH * AUDIO_NUM_CHANNELS * AUDIO_DOUBLE_BUFFER) / 2;
//        for(int i = 0; i < half; i++)
//        {
//            sgtl5000.sai_tx_buffer[half + i] = sgtl5000.sai_rx_buffer[half + i];
//        }
//    }
//}

//// Exemple callback DMA réception moitié terminée
//void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
//{
//    // Copier la première moitié du buffer reçu dans la première moitié du buffer TX
//    memcpy(sgtl5000.sai_tx_buffer, sgtl5000.sai_rx_buffer,
//    		(AUDIO_BUFFER_LENGTH * AUDIO_NUM_CHANNELS * sizeof(int16_t)));
//}
//
//// Callback DMA réception complète
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
    // Copier la deuxième moitié du buffer reçu dans la deuxième moitié du buffer TX
    memcpy(sgtl5000.sai_rx_buffer + AUDIO_BUFFER_LENGTH * AUDIO_NUM_CHANNELS,
           sgtl5000.sai_tx_buffer + AUDIO_BUFFER_LENGTH * AUDIO_NUM_CHANNELS,
           (AUDIO_BUFFER_LENGTH * AUDIO_NUM_CHANNELS * sizeof(int16_t)));
}


