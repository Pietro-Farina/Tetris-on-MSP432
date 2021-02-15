#include "SPI.h"
#include "Screen_HX8353E.h"

Screen_HX8353E screen;
int32_t highscore;

void _init_Screen() {
  screen.begin();
  screen.clear(blackColour);
  highscore = 0;
}

void change_points(int32_t new_value) {
  if (highscore < new_value)
    highscore = new_value;
}

void print_Menu() {
  screen.clear(blackColour);
  screen.gText(7, 20, "T", 0xF800, 0x0000, 3, 3);
  screen.gText(25, 20, "E", 0xF81F, 0x0000, 3, 3);
  screen.gText(43, 20, "T", 0xFFE0, 0x0000, 3, 3);
  screen.gText(61, 20, "R", 0x001F, 0x0000, 3, 3);
  screen.gText(79, 20, "I", 0x07E0, 0x0000, 3, 3);
  screen.gText(97, 20, "S", 0xFFFF, 0x0000, 3, 3);
  screen.gText(7, 65, "HIGHSCORE: " + i32toa(highscore), 0xFFE0, 0x0000, 1, 1);
  screen.gText(10, 100, "Press S1 to START", 0xFFE0, 0x0000, 1, 1);
}

void print_Map() {
  screen.clear(blackColour);
  screen.rectangle(0, 0, 127, 127, 0x7BEF);
  screen.rectangle(1, 1, 126, 126, 0x7BEF);
  screen.rectangle(2, 2, 125, 125, 0x7BEF);
  screen.rectangle(3, 3, 124, 124, 0x7BEF);
}

void draw_Tile(uint16_t x, uint16_t y, uint16_t colour) {
  uint16_t pixel_x1, pixel_y1, pixel_x2, pixel_y2;
  pixel_x1 = (x * 12) + 4;
  pixel_y1 = (y * 12) + 4;
  pixel_x2 = (x * 12) + 15;
  //screen.rectangle(pixel_x1, pixel_y1, pixel_x2, pixel_y2, colour);
  //screen.dRectangle(pixel_x1, pixel_y1, 12, 12, colour);
  if (colour == 0x0000) {
    for (uint16_t i = 0; i < 12; i++) {
      screen.line(pixel_x1, pixel_y1 + i, pixel_x2, pixel_y1 + i, colour);
    }
  }  else {
    screen.dRectangle(pixel_x1, pixel_y1, 12, 12, 0x000F);
    for (uint16_t i = 0; i < 10; i++) {
      screen.line(pixel_x1 + 1, pixel_y1 + 1 + i, pixel_x2 - 1, pixel_y1 + 1 + i, colour);
    }
  }
}

void draw_point(uint16_t x, uint16_t y, uint16_t colour) {
  screen.point(x, y, colour);
}

int printff(int value) {
  draw_point(2, 3, 0xFFFF);
  random(2, 3);
  value++;
  return value;
}
