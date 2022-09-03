#include "../lib/input.h"
#include "../lib/game_constants.h"

static adcsample_t input_samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

void init_input(void) {
  /*
   * Sets up analog input.
   */
  palSetGroupMode(GPIOC, PAL_PORT_BIT(0) | PAL_PORT_BIT(1), 0,
                  PAL_MODE_INPUT_ANALOG);
  adcStart(&ADCD1, NULL);
  adcSTM32EnableTSVREFE();
  palSetLineMode(GREEN_BUTTON_LINE, PAL_MODE_INPUT_PULLDOWN);
}

/*
 * Calculates the speed of the pads
 * depending on the analog input.
 */
void update_pad_speed(game_t *game) {
  while (true) {
    /*
     * Linear conversion.
     */
    adcConvert(&ADCD1, &adcgrpcfg1, input_samples, ADC_GRP1_BUF_DEPTH);
    if ((input_samples[0] - 2048) < -300 || (input_samples[0] - 2048) > 300)
      game->player2.speed_y = ((float)(input_samples[0] - 2048))
          * (PLAYER1_INPUT_MULTIPLIER) * MAX_PAD_SPEED / 2048;
    else
      game->player2.speed_y = 0;
    if ((input_samples[1] - 2048) < -300 || (input_samples[1] - 2048) > 300)
      game->player1.speed_y = ((float)(input_samples[1] - 2048))
          * (PLAYER2_INPUT_MULTIPLIER) * MAX_PAD_SPEED / 2048;
    else
      game->player1.speed_y = 0;
    chThdSleepMilliseconds(UPDATE_TIME/3);
  }
}
