/*
 * sgtl5000_signals.h
 *
 *  Created on: Dec 5, 2025
 *      Author: hugoc
 */

#ifndef INC_SGTL5000_SIGNALS_H_
#define INC_SGTL5000_SIGNALS_H_


#include "sgtl5000.h"
#include "chenille.h"
#include <string.h>

// taille utile : nombre d'échantillons (frames * channels)
#define AUDIO_FRAME_SAMPLES (AUDIO_BUFFER_LENGTH * AUDIO_NUM_CHANNELS)
#define AUDIO_HALF_FRAME_SAMPLES (AUDIO_FRAME_SAMPLES) // on veut 1 période = 1 demi-buffer DMA ici
// note : nos buffers sont dimensionnés AUDIO_FRAME_SAMPLES * AUDIO_DOUBLE_BUFFER


void sgtl5000_fill_triangle(h_sgtl5000_t *h_sgtl5000, int16_t amplitude);


#endif /* INC_SGTL5000_SIGNALS_H_ */
