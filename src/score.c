#include <lib/score.h>

static SSD1306Driver SSD1306D1;

/*
 * Draws horizontal line if "is_on" is set to true.
 */
void draw_horizontal_line(int x, int y, int is_on) {
  if (is_on)
    ssd1306DrawRectangleFill(&SSD1306D1, x, y, SQUARE_LINE_LENGTH,
                             SQUARE_LINE_WIDTH, SSD1306_COLOR_WHITE);
  return;
}
/*
 * Draws vertical line if "is_on" is set to true.
 */
void draw_vertical_line(int x, int y, int is_on) {
  if (is_on)
    ssd1306DrawRectangleFill(&SSD1306D1, x, y, SQUARE_LINE_WIDTH,
                             SQUARE_LINE_LENGTH, SSD1306_COLOR_WHITE);
  return;
}

/*
 * Blueprint for the 7-segment digit, this function
 * takes its position and the rectangles to light up as parameters.
 */
void draw_7_segment(int x, int y, int a, int b, int c, int d, int e, int f,
                    int g) {
  draw_horizontal_line(x + SQUARE_LINE_WIDTH, TITLE_HEIGHT + y, a);
  draw_vertical_line(x, TITLE_HEIGHT + SQUARE_LINE_WIDTH + y, b);
  draw_vertical_line(x + SQUARE_LINE_WIDTH + SQUARE_LINE_LENGTH,
  TITLE_HEIGHT + SQUARE_LINE_WIDTH + y,
                     c);
  draw_horizontal_line(x + SQUARE_LINE_WIDTH,
  TITLE_HEIGHT + SQUARE_LINE_WIDTH + SQUARE_LINE_LENGTH + y,
                       d);
  draw_vertical_line(
      x, y + TITLE_HEIGHT + 2 * SQUARE_LINE_WIDTH + SQUARE_LINE_LENGTH, e);
  draw_vertical_line(
      x + SQUARE_LINE_WIDTH + SQUARE_LINE_LENGTH,
      y + TITLE_HEIGHT + 2 * SQUARE_LINE_WIDTH + SQUARE_LINE_LENGTH, f);
  draw_horizontal_line(
      x + SQUARE_LINE_WIDTH,
      y + TITLE_HEIGHT + 2 * SQUARE_LINE_WIDTH + 2 * SQUARE_LINE_LENGTH, g);
}
/*
 * Draws a digit on the screen,
 * the variable "which" selects the 7-segment digit,
 * the variable "number" is the number to show.
 */
void draw_digit(int which, int number) {
  /*
   * x and y hold the position of the 7-segment digit.
   */
  int x;
  int y = 3;
  switch (which) {
  case 0:
    x = 9;
    break;
  case 1:
    x = 9 + SQUARE_LINE_LENGTH + 2 * SQUARE_LINE_WIDTH + 4;
    break;
  case 2:
    x = SSD1306_WIDTH / 2 + 2 + 9;
    break;
  case 3:
    x = SSD1306_WIDTH / 2 + 2 + 9 + SQUARE_LINE_LENGTH + 2 * SQUARE_LINE_WIDTH
        + 4;
    break;
  }
  /*
   * 1's and 0's are chosen based on the number the 7-segment digit holds.
   */
  switch (number) {
  case 0:
    draw_7_segment(x, y, 1, 1, 1, 0, 1, 1, 1);
    break;
  case 1:
    draw_7_segment(x, y, 0, 0, 1, 0, 0, 1, 0);
    break;
  case 2:
    draw_7_segment(x, y, 1, 0, 1, 1, 1, 0, 1);
    break;
  case 3:
    draw_7_segment(x, y, 1, 0, 1, 1, 0, 1, 1);
    break;
  case 4:
    draw_7_segment(x, y, 0, 1, 1, 1, 0, 1, 0);
    break;
  case 5:
    draw_7_segment(x, y, 1, 1, 0, 1, 0, 1, 1);
    break;
  case 6:
    draw_7_segment(x, y, 1, 1, 0, 1, 1, 1, 1);
    break;
  case 7:
    draw_7_segment(x, y, 1, 0, 1, 0, 0, 1, 0);
    break;
  case 8:
    draw_7_segment(x, y, 1, 1, 1, 1, 1, 1, 1);
    break;
  case 9:
    draw_7_segment(x, y, 1, 1, 1, 1, 0, 1, 1);
    break;
  }
}
/*
 * Updates the score given the players' points.
 */
void score_update(int player1_points, int player2_points) {
  char buff[BUFF_SIZE];

  ssd1306FillScreen(&SSD1306D1, 0x00);
  /*
   * Writes down SCORE in the yellow strip of the display.
   */
  ssd1306GotoXy(&SSD1306D1, 37, 1);
  chsnprintf(buff, BUFF_SIZE, "SCORE");
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE); //writes SCORE on board
  /*
   * Sets up digit numbers.
   */
  int digit0 = player1_points / 10;
  int digit1 = player1_points % 10;
  int digit2 = player2_points / 10;
  int digit3 = player2_points % 10;
  /*
   * Draws digits on the display.
   */
  draw_digit(0, digit0);
  draw_digit(1, digit1);
  draw_digit(2, digit2);
  draw_digit(3, digit3);
  /*
   * Draws line in the middle.
   */
  ssd1306DrawRectangleFill(&SSD1306D1, SSD1306_WIDTH/2 - 1, TITLE_HEIGHT, 2,
                           SSD1306_HEIGHT - TITLE_HEIGHT, SSD1306_COLOR_WHITE);
  /*
   * Updates the display.
   */
  ssd1306UpdateScreen(&SSD1306D1);
}
/*
 * Initializes the score display.
 */
void score_init(void) {
  /*
   * Initialize SSD1306 Display Driver Object.
   */
  ssd1306ObjectInit(&SSD1306D1);
  /*
   * Sets up I2C2_SCL pin.
   */
  palSetLineMode(
      PAL_LINE(GPIOB, 10),
      PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
  /*
   * Sets up I2C2_SDA pin.
   */
  palSetLineMode(
      PAL_LINE(GPIOB, 3),
      PAL_MODE_ALTERNATE(9) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);

  /*
   * Start the SSD1306 Display Driver Object with
   * configuration.
   */
  ssd1306Start(&SSD1306D1, &ssd1306cfg_points);
  /*
   * Starts the score 0-0.
   */
  score_update(0, 0);
}
