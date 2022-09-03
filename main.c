/*
 NeaPolis Innovation Summer Campus 2022 team 1 project
 Copyright (C) 2022 Giovanni Francesco Comune [giovanni.francesco11c@gmail.com]
                    Tammaro Cimmino [tammaro02cimmino@gmail.com]
                    Luca De Filippo [luca.defilippo2@studenti.unina.it]
                    Duilio Barbato [du.barbato@studenti.unina.it]

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * RT-STM32F401RE-NUCLEO64-STMiniPong
 * A recreation of the game "Pong",
 * using the STM32F401RE-NUCLEO64 board, two oled displays,
 * two analog pads, a buzzer and a button.
 */

#include <lib/score.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "ssd1306.h"
#include "stdio.h"
#include "lib/game.h"
#include "lib/visual.h"
#include "lib/input.h"
#include "lib/score.h"
#include "lib/buzzer.h"
#include "lib/music.h"

/**
 * The struct that stores the state of the game. It is shared by all threads.
 */
static game_t game;

static THD_WORKING_AREA(waGame, 2048);
/*
 * This thread updates the game state
 * and pauses the game if the button
 * is pressed.
 */
static THD_FUNCTION(Game, arg) {
  (void)arg;

  chRegSetThreadName("Game");

  start_menu(&game);
  chThdSleepMilliseconds(200);
  while (true) {

    if (palReadLine(GREEN_BUTTON_LINE) == PAL_HIGH) {
      while (palReadLine(GREEN_BUTTON_LINE) == PAL_HIGH) {
      };
      pause_menu(&game);
    }
    else {
      print_game(&game);
      update_game(&game);
      chThdSleepMilliseconds(UPDATE_TIME);
    }
  }

}

/*
 * This thread updates the score on
 * the score display.
 */
static THD_WORKING_AREA(waScore, 512);
static THD_FUNCTION(Score, arg) {
  (void)arg;

  chRegSetThreadName("Score");

  while (true) {

    score_update(game.player2.score, game.player1.score);
    chThdSleepMilliseconds(UPDATE_TIME);
  }

}

/*
 * This thread initializes all peripherals,
 * then collects user input.
 */
int main(void) {

  halInit();
  buzzer_init();
  chSysInit();
  init_display();
  score_init();
  init_input();
  /* Configuring I2C related PINs */

  chThdCreateStatic(waGame, sizeof(waGame), NORMALPRIO, Game, NULL);
  chThdCreateStatic(waScore, sizeof(waScore), NORMALPRIO, Score, NULL);

  while (true) {
    update_pad_speed(&game);
  }
}
