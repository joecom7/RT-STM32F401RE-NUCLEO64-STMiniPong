#include "../lib/game.h"
#include "../lib/visual.h"

static SSD1306Driver SSD1306D1;

/*
 * Initializes the game display.
 */
void init_display(void) {

  palSetLineMode(
      LINE_ARD_D15,
      PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
  palSetLineMode(
      LINE_ARD_D14,
      PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
  ssd1306ObjectInit(&SSD1306D1);

  /*
   * Starts the SSD1306 Display Driver Object with
   * the specified configuration.
   */
  ssd1306Start(&SSD1306D1, &ssd1306cfg_visual);
  ssd1306FillScreen(&SSD1306D1, 0x00);
}

/*
 * Prints the game objects depending on the
 * values inside the game struct.
 */
void print_game_objects(int player1_pos_y, int player2_pos_y, int ball_pos_x,
                        int ball_pos_y) {
  ssd1306FillScreen(&SSD1306D1, 0x00);
  ssd1306DrawLine(&SSD1306D1, PLAYER_DISTANCE_FROM_WALL, player1_pos_y,
                  PLAYER_DISTANCE_FROM_WALL, player1_pos_y+PLAYER_LINE_LENGTH,
                  SSD1306_COLOR_WHITE);
  ssd1306DrawLine(&SSD1306D1, DISPLAY_WIDTH-PLAYER_DISTANCE_FROM_WALL,
                  player2_pos_y, DISPLAY_WIDTH-PLAYER_DISTANCE_FROM_WALL,
                  player2_pos_y+PLAYER_LINE_LENGTH, SSD1306_COLOR_WHITE);
  ssd1306DrawRectangleFill(&SSD1306D1, ball_pos_x, ball_pos_y, BALL_RADIUS,
                           BALL_RADIUS, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);
}

/**
 * Makes the display flash rapidly and plays rapid sounds.
 */
void flash_display(int n) {
  for (int i = 0; i < n; i++) {
    ssd1306ToggleInvert(&SSD1306D1);
    ssd1306UpdateScreen(&SSD1306D1);
    tone_for_milliseconds(500, 2000, 100);
    chThdSleepMilliseconds(100);
    ssd1306ToggleInvert(&SSD1306D1);
    ssd1306UpdateScreen(&SSD1306D1);
    tone_for_milliseconds(500, 2000, 100);
    chThdSleepMilliseconds(100);
  }
}

/*
 * Prints a string to a specified y-coordinate and centers it.
 */

void print_string_centered(char *s, int heigth, const ssd1306_font_t *font) {
  int s_length = (font->fw) * strlength(s);
  int x_string = DISPLAY_WIDTH / 2 - s_length / 2;
  print_string(s, x_string, heigth, font);
}

/*
 * Prints a string to the center
 * of the left or right
 * portion of the screen.
 */
void print_string_centered_side(char *s, int heigth, const ssd1306_font_t *font,
                                side_t side, color_t color) {
  int s_length = (font->fw) * strlength(s);
  int x_string;
  if (side == LEFT_SIDE) {
    x_string = DISPLAY_WIDTH / 4 - s_length / 2;
  }
  else {
    x_string = 3 * DISPLAY_WIDTH / 4 - s_length / 2;
  }
  ssd1306GotoXy(&SSD1306D1, x_string, heigth);
  if (color == BLACK) {
    ssd1306Puts(&SSD1306D1, s, font, SSD1306_COLOR_BLACK);
  }
  else {
    ssd1306Puts(&SSD1306D1, s, font, SSD1306_COLOR_WHITE);
  }
  ssd1306UpdateScreen(&SSD1306D1);
}

/*
 *  Prints a string starting from
 *  the specified x and y coordinates.
 */
void print_string(char *s, int x, int y, const ssd1306_font_t *font) {
  ssd1306GotoXy(&SSD1306D1, x, y);
  ssd1306Puts(&SSD1306D1, s, font, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);
}

/*
 * Sets all the pixels of the
 * game display to black.
 */
void clear_display(void) {
  ssd1306FillScreen(&SSD1306D1, 0x00);
  ssd1306UpdateScreen(&SSD1306D1);
}

/*
 * Returns the length of the string.
 */
int strlength(char *s) {
  int length = 0;
  while (s[length] != '\0')
    length++;
  return length;
}
