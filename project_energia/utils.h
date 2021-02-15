#ifndef __TETRIS_HW_H__
#define __TETRIS_HW_H__

void _init_Random() {
  uint16_t x = analogRead(2);
  uint16_t y = analogRead(26);
  randomSeed((x + 1) * (y + 1));
}

uint8_t random_integer(int min, int max) {
  return random(min, max + 1);
}

uint16_t random_colour() {
  uint16_t res;
  uint8_t random_number;
  random_number = random(0, 6);
  switch (random_number) {
    case 0: res = 0xF800; break; // red
    case 1: res = 0xF81F; break; // magenta
    case 2: res = 0xFFE0; break; // yellow
    case 3: res = 0x001F; break; // blue
    case 4: res = 0x07E0; break; // green
    case 5: res = 0xFFFF; break; // white
    default: break; // error
  }
  return res;
}

#endif /* __TETRIS_HW_H__ */
