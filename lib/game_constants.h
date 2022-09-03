#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

/**
 * Constants used to modify
 * the game objects' behaviour.
 */
#define PLAYER_LINE_LENGTH 10
#define BALL_RADIUS 1
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGTH 64
#define PLAYER_DISTANCE_FROM_WALL 5
#define UPDATE_TIME 20
#define DEFAULT_BALL_SPEED_X -20
#define DEFAULT_BALL_SPEED_Y 0
#define MAX_PAD_SPEED 30
#define MAX_BALL_SPEED_GAIN_ON_COLLISION 20
#define PLAYER1_INPUT_MULTIPLIER 1
#define PLAYER2_INPUT_MULTIPLIER 1
#define START_LOGO "PONG"
#define START_MENU_FIRST_LINE "PRESS BUTTON"
#define START_MENU_SECOND_LINE "TO START"
#define SOUND_VOLUME 1000

/*
 * Line connected to the switch pin of the button.
 */
#define GREEN_BUTTON_LINE PAL_LINE(GPIOA,8)

#endif
