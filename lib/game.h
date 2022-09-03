#ifndef GAME_H
#define GAME_H

#include "game_constants.h"
#include "../lib/visual.h"
#include "../lib/buzzer.h"
#include "../lib/music.h"
#include "ssd1306.h"

/*
 * Enum used to specify
 * whether the player pad
 * is moving.
 */
typedef enum {
  NO_MOTION,
  MOTION,
} motion_t;

/**
 * Enum used to identify
 * the type of collision.
 */
typedef enum {
  NO_COLLISION,
  TOP_COLLISION,
  BOTTOM_COLLISION,
  LEFT_COLLISION,
  RIGHT_COLLISION
} collision_t;

/**
 * Struct used to determine
 * which player scored.
 */
typedef enum {
  PLAYER1_VICTORY,
  PLAYER2_VICTORY,
  NO_VICTORY,
} victory_t;

/*
 * Struct that stores the info
 * about a pad and the player
 * controlling it.
 */
typedef struct {
  float pos_y;
  float speed_y;
  int score;
  motion_t motion;
} player_t;

/*
 * Struct that stores the info
 * about the ball.
 */
typedef struct {
  float pos_x;
  float pos_y;
  float speed_x;
  float speed_y;
} ball_t;

/*
 * Struct that stores the
 * info about the game.
 */

typedef struct game {
  player_t player1;
  player_t player2;
  ball_t ball;
} game_t;

/*
 * Functions prototypes.
 */
void print_game(game_t*); //prints game to display
float new_ball_speed(player_t*, ball_t*);
void update_game(game_t*);
void handle_ball_movement(game_t*);
void handle_pads_movement(game_t*);
collision_t check_collision(game_t*);
void handle_collision(game_t*, collision_t);
void restore_default_game_values(game_t*);
void restore_default_positions_speed(game_t*);
victory_t check_victory(game_t*);
void handle_victory(game_t*, victory_t);
void start_menu(game_t*);
void pause_menu(game_t*);

#endif
