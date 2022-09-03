#ifndef POINTS_H
#define POINTS_H

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "game_constants.h"
#include "visual.h"
#include "ssd1306.h"
#include "stdio.h"

#define SQUARE_LINE_LENGTH 12
#define SQUARE_LINE_WIDTH 4
#define TITLE_HEIGHT 18
#define BUFF_SIZE   10

/*
 * I2C2 configuration.
 */
static const I2CConfig i2ccfg_points = {OPMODE_I2C, 400000, FAST_DUTY_CYCLE_2, };
/*
 * Score SSD1306 configuration.
 */
static const SSD1306Config ssd1306cfg_points = {&I2CD2, &i2ccfg_points,
                                                SSD1306_SAD_0X78, };

/*
 * Functions prototypes.
 */
void draw_horizontal_line(int, int, int);
void draw_vertical_line(int, int, int);
void draw_7_segment(int, int, int, int, int, int, int, int, int);
void draw_digit(int, int);
void score_update(int, int);
void score_init(void);

#endif
