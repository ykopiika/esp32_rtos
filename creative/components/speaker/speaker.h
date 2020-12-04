#ifndef SPEAKER_SPEAKER_H
#define SPEAKER_SPEAKER_H

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include <driver/dac.h>
#include "driver/i2s.h"
#include <string.h>
//#include "freertos/queue.h"

#include "structs.h"
#include "error_functions.h"

#define AMP_POWER_PIN 5
#define SPKR_DELAY 20
#define SPKR_STEP 5

void	init_i2s_speaker(void);
int		sound_on_off(void *ptr);
void	make_bip(void);

#endif
