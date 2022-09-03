#include "../lib/game.h"

/*
 * Points to the thread
 * created to play
 * the background music
 * in the start menu.
 */
static thread_t *thd = NULL;

/*
 * Keeps playing the background
 * music until the thread
 * receives a termination signal.
 */
static THD_WORKING_AREA(waBackgroundMusic, 2048);
static THD_FUNCTION(BackgroundMusic, arg) {
  (void)arg;

  chRegSetThreadName("BackgroundMusic");

  while (!chThdShouldTerminateX())
    play_music(130);

}

void print_game(game_t *game) {
  print_game_objects(game->player1.pos_y, game->player2.pos_y, game->ball.pos_x,
                     game->ball.pos_y);
}

/*
 * Calculates the next state of
 * the game objects (except for the
 * speeds, that are updated in the
 * input thread) depending on:
 * - the positions in the previous state.
 * - the speeds in the previous state.
 */
void update_game(game_t *game) {

  handle_ball_movement(game);
  handle_pads_movement(game);
  victory_t victory = check_victory(game);
  handle_victory(game, victory);
  collision_t collision = check_collision(game);
  handle_collision(game, collision);
}

/*
 * Calculates the next positions of the ball.
 */
void handle_ball_movement(game_t *game) {
  game->ball.pos_x += (game->ball.speed_x) * UPDATE_TIME / 150;
  game->ball.pos_y += (game->ball.speed_y) * UPDATE_TIME / 150;
}

/*
 * Check whether the players are moving and, if they are,
 * calculates the next positions.
 */
void handle_pads_movement(game_t *game) {
  float new_player1_position = game->player1.pos_y
      + (game->player1.speed_y) * UPDATE_TIME / 200;
  /*
   * This check prevents the pads
   * from exceeding the screen limitations.
   */
  if (new_player1_position >= 0
      && new_player1_position < DISPLAY_HEIGTH - PLAYER_LINE_LENGTH - 1) {
    game->player1.pos_y = new_player1_position;
    game->player1.motion = MOTION;
  }
  else {
    game->player1.motion = NO_MOTION; // the pad has a speed that's different from 0
                                      // but it's not really moving.
  }
  float new_player2_position = game->player2.pos_y
      + (game->player2.speed_y) * UPDATE_TIME / 200;
  if (new_player2_position >= 0
      && new_player2_position < DISPLAY_HEIGTH - PLAYER_LINE_LENGTH - 1) {
    game->player2.pos_y = new_player2_position;
    game->player2.motion = MOTION;
  }
  else {
    game->player2.motion = NO_MOTION;
  }
}

/*
 * Restores the default positions
 * of the pads and the ball
 * and the default speeds
 * and sets the scores to 0.
 */
void restore_default_game_values(game_t *game) {
  game->player1.score = 0;
  game->player2.score = 0;
  restore_default_positions_speed(game);
}

/*
 * Restores the default positions
 * of the pads and the ball
 * and the default speeds.
 */
void restore_default_positions_speed(game_t *game) {
  game->player1.pos_y = (DISPLAY_HEIGTH - PLAYER_LINE_LENGTH) / 2;
  game->player2.pos_y = (DISPLAY_HEIGTH - PLAYER_LINE_LENGTH) / 2;
  game->ball.pos_x = DISPLAY_WIDTH / 2;
  game->ball.pos_y = DISPLAY_HEIGTH / 2;
  game->ball.speed_x = DEFAULT_BALL_SPEED_X;
  game->ball.speed_y = DEFAULT_BALL_SPEED_Y;
}

/*
 * Returns the type of collision.
 */
collision_t check_collision(game_t *game) {
  if (game->ball.pos_y <= 0) {
    return TOP_COLLISION;
  }
  if (game->ball.pos_y > DISPLAY_HEIGTH - 1) {
    return BOTTOM_COLLISION;
  }
  if (game->ball.pos_x < PLAYER_DISTANCE_FROM_WALL + 3) {
    if (game->ball.pos_y
        > game->player1.pos_y
            - 3&&game->ball.pos_y<game->player1.pos_y+3+PLAYER_LINE_LENGTH) {
      return LEFT_COLLISION;
    }
  }
  if (game->ball.pos_x > DISPLAY_WIDTH - PLAYER_DISTANCE_FROM_WALL - 3) {
    if (game->ball.pos_y
        > game->player2.pos_y
            - 3&&game->ball.pos_y<game->player2.pos_y+3+PLAYER_LINE_LENGTH) {
      return RIGHT_COLLISION;
    }
  }
  return NO_COLLISION;
}

/*
 * Updates the speed of the ball
 * depending on the type of collision.
 */
void handle_collision(game_t *game, collision_t collision) {
  switch (collision) {
  case TOP_COLLISION:
    game->ball.speed_y *= -1;  //inverts y speed
    break;
  case BOTTOM_COLLISION:
    game->ball.speed_y *= -1;
    break;
  case RIGHT_COLLISION:
    if (game->ball.speed_x > 0)
      game->ball.speed_x *= -1;  // inverts x speed only if
                                 // the ball is moving towards the pad
    (game->ball.speed_y) = new_ball_speed(&(game->player2), &(game->ball));
    tone_for_milliseconds(700, 2000, 200);
    break;
  case LEFT_COLLISION:
    if (game->ball.speed_x < 0)
      game->ball.speed_x *= -1;
    (game->ball.speed_y) = new_ball_speed(&(game->player1), &(game->ball));
    tone_for_milliseconds(400, 2000, 200);
    break;
  case NO_COLLISION:
    break;
  }
}

/*
 * Checks if a player scored a point.
 */
victory_t check_victory(game_t *game) {
  if (game->ball.pos_x < PLAYER_DISTANCE_FROM_WALL - 3) {
    return PLAYER1_VICTORY;
  }
  if (game->ball.pos_x > DISPLAY_WIDTH - PLAYER_DISTANCE_FROM_WALL + 3) {
    return PLAYER2_VICTORY;
  }
  return NO_VICTORY;
}

/*
 * If a player scored a point, this function
 * updates the player's score,
 * makes the screen flash
 * and restores the default positions and speeds.
 */
void handle_victory(game_t *game, victory_t victory) {
  if (victory == NO_VICTORY)
    return;
  flash_display(2);
  if (victory == PLAYER1_VICTORY && game->player1.score != 99) {
    game->player1.score += 1;
  }
  else if (victory == PLAYER2_VICTORY && game->player2.score != 99) {
    game->player2.score += 1;
  }
  restore_default_positions_speed(game);
  chThdSleepMilliseconds(1000);
}

/**
 * Calculates and sets the speed of the ball
 * after the collision.
 */
float new_ball_speed(player_t *player, ball_t *ball) {
  /*
   * If the pad is moving when it hits
   * the ball, the ball takes half
   * of the pad's speed.
   */
  if (player->motion == NO_MOTION
      || (player->speed_y > -10 && player->speed_y < 10))
    return ball->speed_y / 2;
  float new_speed = (player->speed_y
      * ((float)MAX_BALL_SPEED_GAIN_ON_COLLISION / MAX_PAD_SPEED));
  if (player->speed_y * ball->speed_y < 0)
    return new_speed * 0.5;
  return new_speed * 1.5;
}

/*
 * Implements a start menu.
 */
void start_menu(game_t *game) {
  thd = chThdCreateStatic(waBackgroundMusic, sizeof(waBackgroundMusic),
  NORMALPRIO,
                          BackgroundMusic, NULL);
  restore_default_game_values(game);
  int isMessageDisplayed = 0;
  for (int i = 40; i >= 0; i--) {
    clear_display();
    print_string_centered(START_LOGO, i, &ssd1306_font_11x18);
    chThdSleepMilliseconds(20);
  }
  while (1) {
    if (!isMessageDisplayed) {
      print_string_centered(START_MENU_FIRST_LINE, 30, &ssd1306_font_7x10);
      print_string_centered(START_MENU_SECOND_LINE, 50, &ssd1306_font_7x10);
      isMessageDisplayed = 1;
    }
    else {
      clear_display();
      print_string_centered(START_LOGO, 0, &ssd1306_font_11x18);
      isMessageDisplayed = 0;
    }
    chThdSleepMilliseconds(20);
    for (int i = 20; i <= 300; i += 20) {
      if (palReadLine(GREEN_BUTTON_LINE) == PAL_HIGH) {
        chThdTerminate(thd);
        while (palReadLine(GREEN_BUTTON_LINE) == PAL_HIGH) {
        }
        return;
      }
      chThdSleepMilliseconds(20);
    }
  }

}

/*
 * Implements a pause menu.
 */
void pause_menu(game_t *game) {
  tone_for_milliseconds(380, 2000, 100);
  clear_display();
  chThdSleepMilliseconds(100);
  side_t selected_side = LEFT_SIDE;
  print_string_centered("PAUSE MENU", 0, &ssd1306_font_11x18);
  print_string_centered_side("RESUME", 30, &ssd1306_font_11x18, LEFT_SIDE,
                             BLACK);
  print_string_centered_side("RESET", 30, &ssd1306_font_11x18, RIGHT_SIDE,
                             WHITE);
  while (1) {
    if (selected_side == LEFT_SIDE) {
      if ((game->player1.speed_y <= -3 * MAX_PAD_SPEED / 4)
          || (game->player2.speed_y <= -3 * MAX_PAD_SPEED / 4)) {
        tone_for_milliseconds(300, 2000, 100);
        print_string_centered_side("RESUME", 30, &ssd1306_font_11x18, LEFT_SIDE,
                                   WHITE);
        print_string_centered_side("RESET", 30, &ssd1306_font_11x18, RIGHT_SIDE,
                                   BLACK);
        selected_side = RIGHT_SIDE;
        continue;
      }
      if (palReadLine(GREEN_BUTTON_LINE) == PAL_HIGH) {
        tone_for_milliseconds(380, 2000, 100);
        while (palReadLine(GREEN_BUTTON_LINE) == PAL_HIGH) {
        }
        chThdSleepMilliseconds(20);
        return;
      }
    }
    else {
      if ((game->player1.speed_y >= 3 * MAX_PAD_SPEED / 4)
          || (game->player2.speed_y >= 3 * MAX_PAD_SPEED / 4)) {
        tone_for_milliseconds(300, 2000, 100);
        print_string_centered_side("RESUME", 30, &ssd1306_font_11x18, LEFT_SIDE,
                                   BLACK);
        print_string_centered_side("RESET", 30, &ssd1306_font_11x18, RIGHT_SIDE,
                                   WHITE);
        selected_side = LEFT_SIDE;
        continue;
      }
      if (palReadLine(GREEN_BUTTON_LINE) == PAL_HIGH) {
        while (palReadLine(GREEN_BUTTON_LINE) == PAL_HIGH) {
        }
        start_menu(game);
        return;
      }
    }
    chThdSleepMilliseconds(20);
  }
}
