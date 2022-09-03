#include "../lib/buzzer.h"
#include "../lib/game_constants.h"

/*
 * Points to the thread
 * created to play sounds.
 */
static thread_t *thd = NULL;

/*
 * Notes and duration
 * for the start menu song.
 * source: https://github.com/robsoncouto/arduino-songs/blob/master/vampirekiller/vampirekiller.ino
 */
const int melody[] = {

NOTE_DS4,
                      -8, NOTE_FS4, -8, NOTE_C5, 8, NOTE_B4, -8, NOTE_G4, -8,
                      NOTE_E4,
                      8,
                      NOTE_DS4,
                      -8,
                      NOTE_FS4,
                      -8,
                      NOTE_C5,
                      8,
                      NOTE_B4,
                      -8,
                      NOTE_CS5,
                      -8,
                      NOTE_DS5,
                      8,

                      NOTE_E5,
                      16,
                      NOTE_E5,
                      16,
                      NOTE_E4,
                      16,
                      NOTE_E4,
                      -2, //18
                      NOTE_C4, 8, NOTE_C4, 8, NOTE_E4, 16, NOTE_G4, -8, NOTE_D4,
                      8, NOTE_D4, 8,
                      NOTE_FS4,
                      16, NOTE_A4, -8,
                      NOTE_E5,
                      16, NOTE_E5, 16, NOTE_E4, 16, NOTE_E4, -2,
                      NOTE_C4,
                      8, NOTE_C4, 8, NOTE_E4, 16, NOTE_G4, -8, NOTE_D4, 8,
                      NOTE_D4,
                      8, NOTE_B3, 16, NOTE_D4, -8,

                      NOTE_E5,
                      16, NOTE_E5, 8, NOTE_D5, 16, REST, 16, NOTE_CS5, -4,
                      NOTE_E4,
                      8, NOTE_FS4, 16,
                      NOTE_G4,
                      16,
                      NOTE_A4,
                      16,

                      NOTE_B4,
                      -8,
                      NOTE_E4,
                      -8,
                      NOTE_B4,
                      8,
                      NOTE_A4,
                      16,
                      NOTE_D5,
                      -4, //7
                      NOTE_E5, 16, NOTE_E5, 8, NOTE_D5, 16, REST, 16, NOTE_CS5,
                      -4, NOTE_E4, 8,
                      NOTE_FS4,
                      16, NOTE_G4, 16, NOTE_A4, 16,
                      NOTE_B4,
                      -8, NOTE_E4, -8, NOTE_B4, 8, NOTE_A4, 16, NOTE_D4, -4,
                      REST,
                      8, NOTE_E5, 8, REST, 16, NOTE_B5, 16, REST, 8, NOTE_AS5,
                      16, NOTE_B5, 16,
                      NOTE_AS5,
                      16, NOTE_G5, 16, REST, 4,

                      NOTE_B5,
                      8, NOTE_B5, 16, NOTE_AS5, 16, REST, 16, NOTE_AS5, 16,
                      NOTE_A5,
                      16, REST, 16,
                      NOTE_B5,
                      16, NOTE_G5, 16, NOTE_B5, 16, NOTE_AS5, 16,
                      REST,
                      16,
                      NOTE_B5,
                      16,
                      NOTE_A5,
                      16,
                      NOTE_G5,
                      16, //11
                      REST, 8, NOTE_E5, 8, REST, 16, NOTE_B5, 16, REST, 8,
                      NOTE_AS5,
                      16, NOTE_B5, 16, NOTE_AS5, 16, NOTE_G5, 16,
                      REST,
                      4,
                      NOTE_B5,
                      8, NOTE_B5, 16, NOTE_AS5, 16, REST, 16, NOTE_AS5, 16,
                      NOTE_A5,
                      16, REST, 16,
                      NOTE_B5,
                      16, NOTE_G5, 16, NOTE_B5, 16, NOTE_AS5, 16, REST, 16,
                      NOTE_B5,
                      16,
                      NOTE_A5,
                      16, NOTE_G5, 16,
                      NOTE_DS4,
                      -8, NOTE_FS4, -8, NOTE_C5, 8, NOTE_B4, -8, NOTE_G4, -8,
                      NOTE_E4,
                      8,
                      NOTE_DS4,
                      -8, NOTE_FS4, -8, NOTE_C5, 8, NOTE_B4, -8, NOTE_CS5, -8,
                      NOTE_DS5,
                      8

};

/*
 * This thread allows to play
 * sound effects for a specific
 * duration without impacting
 * other threads.
 */
static THD_WORKING_AREA(waSoundEffect, 1024);
static THD_FUNCTION(SoundEffect, arg) {
  sound_t *sound = (sound_t*)arg;

  chRegSetThreadName("SoundEffect");

  tone(sound->frequency, sound->volume);
  chThdSleepMilliseconds(sound->duration_milliseconds);
  noTone();
  chThdExit(MSG_OK);
}

/*
 * The number of notes in the song.
 */
const int notes_number = sizeof(melody) / sizeof(melody[0]) / 2;

/*
 * PWM Configuration.
 */
static PWMConfig pwmcfg = {
PWM_FREQ,
                           PWM_PERIOD, NULL, { {PWM_OUTPUT_ACTIVE_LOW, NULL}, {
                               PWM_OUTPUT_DISABLED, NULL},
                                              {PWM_OUTPUT_DISABLED, NULL},
                                              {
                                              PWM_OUTPUT_DISABLED,
                                               NULL}},
                           0, 0, 1};

/*
 * Sets up the pwm.
 */
void buzzer_init(void) {
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(2)); // TIM3_CH1

  // Start PWM Driver on Timer 3
  pwmStart(&PWMD3, &pwmcfg);
}

/*
 * Plays a note given frequency and volume.
 */
void tone(int freq, int vol) {
  if (freq == 0) {
    noTone();
  }
  else {
    pwmChangePeriod(&PWMD3, 1000000 / freq);
    pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, vol));
  }
}

/*
 * Shuts the buzzer.
 */
void noTone(void) {
  pwmDisableChannel(&PWMD3, 0);
}

/*
 * Plays a song.
 * The notes and durations are stored
 * in the melody array.
 * Tempo is the speed in bpm.
 */
void play_music(int tempo) {
  int wholenote = (60000 * 4) / tempo;

  int divider = 0, noteDuration = 0;
  /*
   * Keeps playing only if the thread
   * has not received a termination
   * signal from its parent!
   */
  for (int i = 0; i < notes_number && (!chThdShouldTerminateX()); i += 2) {
    divider = melody[i + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    }
    else if (divider < 0) {
      // dotted note
      noteDuration = (wholenote) / (divider);
      noteDuration *= -1.5;
    }

    tone(melody[i], SOUND_VOLUME);
    chThdSleepMilliseconds(9 * noteDuration / 10);

    noTone();
    chThdSleepMilliseconds(noteDuration / 10);
  }
}

/*
 * Plays a note for a specific time
 * in a separated thread.
 */
void tone_for_milliseconds(int freq, int vol, int milliseconds) {
  sound_t sound;
  sound.frequency = freq;
  sound.volume = vol;
  sound.duration_milliseconds = milliseconds;
  if (thd == NULL || thd->state == CH_STATE_FINAL)
    thd = chThdCreateStatic(waSoundEffect, sizeof(waSoundEffect),
    NORMALPRIO + 1,
                            SoundEffect, (void*)&sound);
}
