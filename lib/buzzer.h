#ifndef BUZZER_H
#define BUZZER_H

#include "ch.h"
#include "hal.h"
#include "music.h"

#define PWM_FREQ    1000000    // PWM Timer Frequency
#define PWM_PERIOD  100     // PWM Period

#define BUZ_LINE PAL_LINE(GPIOA, 6)

/*
 * Stores info about the sound
 * to be played on the buzzer.
 */
typedef struct {
  int frequency;
  int volume;
  int duration_milliseconds;
} sound_t;

/*
 * Function prototypes.
 */
void buzzer_init(void);
void tone(int, int);
void tone_for_milliseconds(int, int, int);
void noTone(void);
void play_music(int);
#endif
