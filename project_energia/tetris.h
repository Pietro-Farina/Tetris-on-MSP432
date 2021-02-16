//#ifndef __TETRIS_H__
//#define __TETRIS_H__

#define MAX_TILES 4
#define MAX_COLUMNS 10
#define MAX 10
#define BUFFER_SIZE 101
#define POINTS_FOR_ROW 30
#define POINTS_FOR_FASTFALL 8
#define POINTS_FOR_FALL 1

typedef enum Direction {R, L, D} Direction;
typedef enum {unknown, I, J, O, S, Z, T} Shape;
//typedef enum {false, true} boolean;

typedef struct  {
  uint8_t buf[BUFFER_SIZE];
  uint8_t head;
  uint8_t tail;
} Buffer;

typedef struct {
  int8_t x;
  int8_t y;
} Tile;

typedef struct {
  uint16_t colour;
  int8_t center_x;
  int8_t center_y;
  uint8_t nr_tiles;
  uint8_t nr_rotation;
  Shape shape;
  Tile tiles[MAX_TILES];
} Piece;

typedef struct {
  uint32_t points;
  int8_t int_matrix[MAX][MAX];       // Tiene id ma deve tenere anche colore
  uint16_t char_matrix[MAX][MAX];
  uint8_t row_occupation[MAX_COLUMNS];
  Buffer avaible_id;
  Piece moving_Piece;
} Map;
//typedef struct Map Map;



//----------------------- FUNCTION FOR HANDLING THE BUFFER -----------------------//

void initialize_empty_Buffer(Buffer* buffer) {
  buffer->head = 0;
}

void initialize_full_Buffer(Buffer* buffer, uint8_t starting_value) {
  for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
    buffer->buf[i] = starting_value + i;
  }
  buffer->buf[BUFFER_SIZE - 1] = 0;
  buffer->head = 0;
  buffer->tail = BUFFER_SIZE - 1;
}

bool empty_buffer(Buffer* buffer) {
  return buffer->head == buffer->tail;
}

bool full_buffer(Buffer* buffer) {
  return (buffer->tail + 1) % BUFFER_SIZE == buffer->head;
}

void add_to_int_buffer(Buffer* buffer, uint8_t value) {
  buffer->buf[buffer->tail++] = value;
  if (buffer->tail == BUFFER_SIZE)
    buffer->tail = 0;
}

uint8_t get_from_int_buffer(Buffer* buffer) {
  uint8_t res;
  res = buffer->buf[buffer->head++];
  if (buffer->head == BUFFER_SIZE)
    buffer->head = 0;
  return res;
}



//----------------------- FUNCTION FOR INTIALIZING THE GAME -----------------------//

void initialize_int_matrix(int8_t matrix[MAX][MAX]) {
  for (int8_t i = 0, j = 0; i < MAX; i++) {
    for (j = 0; j < MAX; j++) {
      matrix[i][j] = 0;
    }
  }
}

void initialize_char_matrix(uint16_t matrix[MAX][MAX]) {
  for (int8_t i = 0, j = 0; i < MAX; i++) {
    for (j = 0; j < MAX; j++) {
      matrix[i][j] = 0x0000;
    }
  }
}

void initialize_Tile(Tile* tile) {
  tile->x = -1;
  tile->y = -1;
}

void initialize_Piece(Piece* piece) {
  piece->colour = 0x0000;
  piece->shape = unknown;
  piece->center_x = -1;
  piece->center_y = -1;
  piece->nr_tiles = 0;
  piece->nr_rotation = 0;
  for (uint8_t i = 0; i < MAX_TILES; i++) {
    initialize_Tile(&(piece->tiles[i]));
  }
}

/*
    initialize all field of the Map
*/
void initialize_Map(Map* map) {
  for (uint8_t i = 0; i < MAX_COLUMNS; i++) {
    map->row_occupation[i] = 0;
  }
  initialize_int_matrix(map->int_matrix);
  initialize_char_matrix(map->char_matrix);
  initialize_Piece(&(map->moving_Piece));
  initialize_full_Buffer(&(map->avaible_id), 1);
  map->points = 0;
}




//----------------------- FUNCTION FOR HANDLING THE GAME -----------------------//

/*
    Overwrite the value with a new Piece. It returns 1 if there is no space for the pieced (gameover), otherwise 0
*/
uint8_t new_Piece(Map* map, Shape shape) {
  // The piece should be initialized or dead

  // if shape is unknown then I get a random number and assign it, otherwise the asked shape will be created
  if (shape == unknown) {
    shape = (Shape) random_integer(1, 6);
  }
  map->moving_Piece.shape = shape;

  // switch case(shape) and assign the right tiles
  switch (map->moving_Piece.shape) {
    case 1: { // I
        map->moving_Piece.nr_tiles = 3;
        map->moving_Piece.tiles[0].x = 3;
        map->moving_Piece.tiles[0].y = 0;
        map->moving_Piece.tiles[1].x = 4;
        map->moving_Piece.tiles[1].y = 0;
        map->moving_Piece.tiles[2].x = 5;
        map->moving_Piece.tiles[2].y = 0;
      }; break;
    case 2: { // J
        map->moving_Piece.nr_tiles = 3;
        map->moving_Piece.tiles[0].x = 4;
        map->moving_Piece.tiles[0].y = 0;
        map->moving_Piece.tiles[1].x = 5;
        map->moving_Piece.tiles[1].y = 0;
        map->moving_Piece.tiles[2].x = 5;
        map->moving_Piece.tiles[2].y = 1;
      }; break;
    case 3: { // O
        map->moving_Piece.nr_tiles = 4;
        map->moving_Piece.tiles[0].x = 4;
        map->moving_Piece.tiles[0].y = 0;
        map->moving_Piece.tiles[1].x = 5;
        map->moving_Piece.tiles[1].y = 0;
        map->moving_Piece.tiles[2].x = 4;
        map->moving_Piece.tiles[2].y = 1;
        map->moving_Piece.tiles[3].x = 5;
        map->moving_Piece.tiles[3].y = 1;
      } break;
    case 4: { // S
        map->moving_Piece.nr_tiles = 4;
        map->moving_Piece.tiles[0].x = 3;
        map->moving_Piece.tiles[0].y = 1;
        map->moving_Piece.tiles[1].x = 4;
        map->moving_Piece.tiles[1].y = 1;
        map->moving_Piece.tiles[2].x = 4;
        map->moving_Piece.tiles[2].y = 0;
        map->moving_Piece.tiles[3].x = 5;
        map->moving_Piece.tiles[3].y = 0;
      }; break;
    case 5: { // Z
        map->moving_Piece.nr_tiles = 4;
        map->moving_Piece.tiles[0].x = 3;
        map->moving_Piece.tiles[0].y = 0;
        map->moving_Piece.tiles[1].x = 4;
        map->moving_Piece.tiles[1].y = 0;
        map->moving_Piece.tiles[2].x = 4;
        map->moving_Piece.tiles[2].y = 1;
        map->moving_Piece.tiles[3].x = 5;
        map->moving_Piece.tiles[3].y = 1;
      }; break;
    case 6: { // T
        map->moving_Piece.nr_tiles = 4;
        map->moving_Piece.tiles[0].x = 3;
        map->moving_Piece.tiles[0].y = 0;
        map->moving_Piece.tiles[1].x = 4;
        map->moving_Piece.tiles[1].y = 0;
        map->moving_Piece.tiles[2].x = 5;
        map->moving_Piece.tiles[2].y = 0;
        map->moving_Piece.tiles[3].x = 4;
        map->moving_Piece.tiles[3].y = 1;
      }; break;
    default: break; // ERROR, SHOULD NEVER COME HERE
  }

  // assignment of values not depending on shape: center's coordinates and colour
  map->moving_Piece.center_x = 4;
  map->moving_Piece.center_y = 0;
  map->moving_Piece.nr_rotation = 0;
  map->moving_Piece.colour = random_colour();

  // array[row][column] column = x, row = y
  int8_t x, y;
  bool nospace = false;
  uint16_t colour;
  for (uint8_t i = 0; i < map->moving_Piece.nr_tiles; ++i) {
    x = map->moving_Piece.tiles[i].x;
    y = map->moving_Piece.tiles[i].y;
    if (map->int_matrix[y][x] != 0) {
      nospace = true;
    } else {
      colour = map->moving_Piece.colour;
      map->int_matrix[y][x] = -1;
      map->char_matrix[y][x] = colour;
      //  UPDATE PIXELS
      draw_Tile(x, y, colour);
    }
  }
  if (nospace)  // gameover
    return 1;
  return 0;
}

/*
   It receive the new position for the tiles and update the piece and the map.
   pos[] is the array of coordinates: {{x1, y1}, {x2, y2}, ...} which will be replace the ones of the moving Piece.
*/
void update_Tiles(Map* map, int8_t pos[][2]) {
  uint16_t colour;
  uint8_t i;
  int8_t x, y, new_x, new_y;
  for (i = 0; i < map->moving_Piece.nr_tiles; ++i) {
    x = map->moving_Piece.tiles[i].x;
    y = map->moving_Piece.tiles[i].y;
    new_x = pos[i][0];
    new_y = pos[i][1];

    // modify the tiles I leave
    map->int_matrix[y][x] += 1;

    // modify the tiles I come
    map->int_matrix[new_y][new_x] -= 1;
  }
  // noInterrupts()
  for (i = 0; i < map->moving_Piece.nr_tiles; ++i)
  {
    x = map->moving_Piece.tiles[i].x;
    y = map->moving_Piece.tiles[i].y;
    new_x = pos[i][0];
    new_y = pos[i][1];
    colour = map->moving_Piece.colour;

    if (map->int_matrix[new_y][new_x] == -1) {
      map->char_matrix[new_y][new_x] = colour;
      draw_Tile(new_x, new_y, colour);
    }
    if (map->int_matrix[y][x] == 0) {
      map->char_matrix[y][x] = 0x0000;
      draw_Tile(x, y, 0x0000);
    }

    // modify the tiles in the Pice
    map->moving_Piece.tiles[i].x = new_x;
    map->moving_Piece.tiles[i].y = new_y;

    //  UPDATE PIXELS
  }
  // interrupts()
}


/*
    Check the tiles the rotated Piece would occupy, if empty it rotates, otherwise do nothing
    The space involved in the rotation depends on the shape of the piece, if is near to edges and the
    number of previous rotation
*/
void rotate_Piece(Map* map) {
  int8_t center_x, center_y;
  bool can_move;
  can_move = true;
  center_x = map->moving_Piece.center_x;
  center_y = map->moving_Piece.center_y;
  switch (map->moving_Piece.shape) {
    case 0: break;
    case 1: { // I
        // check for edges case
        if (center_x == 0) {
          if ((map->int_matrix[center_y][center_x + 1] > 0) || (map->int_matrix[center_y][center_x + 2] > 0))
            can_move = false;
          if (can_move) {
            // move tiles
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x + 1), center_y}, {(int8_t) (center_x + 2), center_y}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_x++;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_x == MAX - 1) {
          if ((map->int_matrix[center_y][center_x - 1] > 0) || (map->int_matrix[center_y][center_x - 2] > 0))
            can_move = false;
          if (can_move) {
            // move tiles
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), center_y}, {(int8_t) (center_x - 2), center_y}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_x--;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_y == 0) {
          if ((map->int_matrix[center_y + 1][center_x] > 0) || (map->int_matrix[center_y + 2][center_x] > 0))
            can_move = false;
          if (can_move) {
            // move tiles
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y + 1)}, {center_x, (int8_t) (center_y + 2)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_y++;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_y == MAX - 1) {
          if ((map->int_matrix[center_y - 1][center_x] > 0) || (map->int_matrix[center_y - 2][center_x] > 0))
            can_move = false;
          if (can_move) {
            // move tiles
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y - 1)}, {center_x, (int8_t) (center_y - 2)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_y--;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else {  // we are not in an edge situation
          switch (map->moving_Piece.nr_rotation % 2) {
            case 0: {
                if ((map->int_matrix[center_y - 1][center_x] > 0) || (map->int_matrix[center_y + 1][center_x] > 0))
                  can_move = false;
                if (can_move) {
                  // move tiles
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y - 1)}, {center_x, (int8_t) (center_y + 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 1: {
                if ((map->int_matrix[center_y][center_x - 1] > 0) || (map->int_matrix[center_y][center_x + 1] > 0))
                  can_move = false;
                if (can_move) {
                  // move tiles
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), center_y}, {(int8_t) (center_x + 1), center_y}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            default: break; // error
          }
        }
      }; break;
    case 2: { // J
        // there are no conflict with edges so we watch only normal case
        switch (map->moving_Piece.nr_rotation % 4) {
          case 0: {
              if (map->int_matrix[center_y + 1][center_x] > 0)
                can_move = false;
              if (can_move) {
                // update map
                int8_t pos[][2] = {{(int8_t) (center_x + 1), center_y}, {center_x, (int8_t) (center_y + 1)}, {(int8_t) (center_x + 1), (int8_t) (center_y + 1)}};
                update_Tiles(map, pos);

                // update center
                map->moving_Piece.center_x++;
                // update rotation
                map->moving_Piece.nr_rotation++;
              }
            }; break;
          case 1: {
              if (map->int_matrix[center_y][center_x - 1] > 0)
                can_move = false;
              if (can_move) {
                // update map
                int8_t pos[][2] = {{center_x, (int8_t) (center_y + 1)}, {(int8_t) (center_x - 1), (int8_t) (center_y + 1)}, {(int8_t) (center_x - 1), center_y}};
                update_Tiles(map, pos);

                // update center
                map->moving_Piece.center_y++;
                // update rotation
                map->moving_Piece.nr_rotation++;
              }
            }; break;
          case 2: {
              if (map->int_matrix[center_y - 1][center_x] > 0)
                can_move = false;
              if (can_move) {
                // update map
                int8_t pos[][2] = {{(int8_t) (center_x - 1), center_y}, {(int8_t) (center_x - 1), (int8_t) (center_y - 1)}, {center_x, (int8_t) (center_y - 1)}};
                update_Tiles(map, pos);

                // update center
                map->moving_Piece.center_x--;
                // update rotation
                map->moving_Piece.nr_rotation++;
              }
            }; break;
          case 3: {
              if (map->int_matrix[center_y][center_x + 1] > 0)
                can_move = false;
              if (can_move) {
                // update map
                int8_t pos[][2] = {{center_x, (int8_t) (center_y - 1)}, {(int8_t) (center_x + 1), (int8_t) (center_y - 1)}, {(int8_t) (center_x + 1), center_y}};
                update_Tiles(map, pos);

                // update center
                map->moving_Piece.center_y--;
                // update rotation
                map->moving_Piece.nr_rotation++;
              }
            }; break;
          default: break;
        }
      }; break;
    case 3: { // O
      }; break;
    case 4: { // S
        // check for edges case
        if (center_x == 0) {
          if ((map->int_matrix[center_y][center_x + 2] > 0) || (map->int_matrix[center_y + 1][center_x] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{(int8_t) (center_x + 1), center_y}, {(int8_t) (center_x + 2), center_y}, {center_x, (int8_t) (center_y + 1)}, {(int8_t) (center_x + 1), (int8_t) (center_y + 1)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_x++;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_x == MAX - 1) {
          if ((map->int_matrix[center_y - 1][center_x] > 0) || (map->int_matrix[center_y][center_x - 2] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{(int8_t) (center_x - 1), center_y}, {(int8_t) (center_x - 2), center_y}, {center_x, (int8_t) (center_y - 1)}, {(int8_t) (center_x - 1), (int8_t) (center_y - 1)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_x--;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_y == 0) {
          if ((map->int_matrix[center_y][center_x - 1] > 0) || (map->int_matrix[center_y + 2][center_x] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{(int8_t) (center_x - 1), center_y}, {center_x, (int8_t) (center_y + 1)}, {(int8_t) (center_x - 1), (int8_t) (center_y + 1)}, {center_x, (int8_t) (center_y + 2)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_y++;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_y == MAX - 1) {
          if ((map->int_matrix[center_y][center_x + 1] > 0) || (map->int_matrix[center_y - 2][center_x] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, (int8_t) (center_y - 1)}, {center_x, (int8_t) (center_y - 2)}, {(int8_t) (center_x + 1), center_y}, {(int8_t) (center_x + 1), (int8_t) (center_y - 1)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_y--;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else {  // we are not in an edge situation
          switch (map->moving_Piece.nr_rotation % 4) {
            case 0: {
                if ((map->int_matrix[center_y - 1][center_x - 1] > 0) || (map->int_matrix[center_y][center_x - 1] > 0))
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y + 1)}, {(int8_t) (center_x - 1), center_y}, {(int8_t) (center_x - 1), (int8_t) (center_y - 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 1: {
                if ((map->int_matrix[center_y - 1][center_x] > 0) || (map->int_matrix[center_y - 1][center_x + 1] > 0))
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y - 1)}, {(int8_t) (center_x + 1), (int8_t) (center_y - 1)}, {(int8_t) (center_x - 1), center_y}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 2: {
                if ((map->int_matrix[center_y][center_x + 1] > 0) || (map->int_matrix[center_y + 1][center_x + 1] > 0))
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y - 1)}, {(int8_t) (center_x + 1), center_y}, {(int8_t) (center_x + 1), (int8_t) (center_y + 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 3: {
                if ((map->int_matrix[center_y + 1][center_x] > 0) || (map->int_matrix[center_y + 1][center_x - 1] > 0))
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x + 1), center_y}, {center_x, (int8_t) (center_y + 1)}, {(int8_t) (center_x - 1), (int8_t) (center_y + 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            default: break; // error
          }
        }
      }; break;
    case 5: { // Z
        // check for edges case
        if (center_x == 0) {
          if ((map->int_matrix[center_y + 1][center_x + 1] > 0) || (map->int_matrix[center_y + 1][center_x + 2] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x + 1), center_y}, {(int8_t) (center_x + 1), (int8_t) (center_y + 1)}, {(int8_t) (center_x + 2), (int8_t) (center_y + 1)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_x++;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_x == MAX - 1) {
          if ((map->int_matrix[center_y - 1][center_x - 1] > 0) || (map->int_matrix[center_y - 1][center_x - 2] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), center_y}, {(int8_t) (center_x - 1), (int8_t) (center_y - 1)}, {(int8_t) (center_x - 2), (int8_t) (center_y - 1)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_x--;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_y == 0) {
          if ((map->int_matrix[center_y + 1][center_x - 1] > 0) || (map->int_matrix[center_y + 2][center_x - 1] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y + 1)}, {(int8_t) (center_x - 1), (int8_t) (center_y + 1)}, {(int8_t) (center_x - 1), (int8_t) (center_y + 2)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_y++;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_y == MAX - 1) {
          if ((map->int_matrix[center_y - 1][center_x + 1] > 0) || (map->int_matrix[center_y - 2][center_x + 1] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y - 1)}, {(int8_t) (center_x + 1), (int8_t) (center_y - 1)}, {(int8_t) (center_x + 1), (int8_t) (center_y - 2)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_y--;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else {  // we are not in an edge situation
          switch (map->moving_Piece.nr_rotation % 4) {
            case 0: {
                if ((map->int_matrix[center_y - 1][center_x] > 0) || (map->int_matrix[center_y + 1][center_x - 1] > 0))
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), (int8_t) (center_y)}, {(int8_t) (center_x - 1), (int8_t) (center_y + 1)}, {center_x, (int8_t) (center_y - 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 1: {
                if ((map->int_matrix[center_y - 1][center_x - 1] > 0) || (map->int_matrix[center_y][center_x + 1] > 0))
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), (int8_t) (center_y - 1)}, {center_x, (int8_t) (center_y - 1)}, {(int8_t) (center_x + 1), center_y}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 2: {
                if ((map->int_matrix[center_y - 1][center_x + 1] > 0) || (map->int_matrix[center_y + 1][center_x] > 0))
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x + 1), (int8_t) (center_y - 1)}, {(int8_t) (center_x + 1), center_y}, {center_x, (int8_t) (center_y + 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 3: {
                if ((map->int_matrix[center_y][center_x - 1] > 0) || (map->int_matrix[center_y + 1][center_x + 1] > 0))
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), center_y}, {center_x, (int8_t) (center_y + 1)}, {(int8_t) (center_x + 1), (int8_t) (center_y + 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            default: break; // error
          }
        }
      }; break;
    case 6: { // T
        // check for edges case
        if (center_x == 0) {
          if ((map->int_matrix[center_y][center_x + 2] > 0) || (map->int_matrix[center_y + 1][center_x + 1] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x + 1), center_y}, {(int8_t) (center_x + 1), (int8_t) (center_y + 1)}, {(int8_t) (center_x + 2), center_y}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_x++;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_x == MAX - 1) {
          if ((map->int_matrix[center_y - 1][center_x - 1] > 0) || (map->int_matrix[center_y][center_x - 2] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), center_y}, {(int8_t) (center_x - 1), (int8_t) (center_y - 1)}, {(int8_t) (center_x - 2), center_y}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_x--;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_y == 0) {
          if ((map->int_matrix[center_y + 1][center_x - 1] > 0) || (map->int_matrix[center_y + 2][center_x] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y + 1)}, {center_x, (int8_t) (center_y + 2)}, {(int8_t) (center_x - 1), (int8_t) (center_y + 1)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_y++;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else if (center_y == MAX - 1) {
          if ((map->int_matrix[center_y - 1][center_x + 1] > 0) || (map->int_matrix[center_y - 2][center_x] > 0))
            can_move = false;
          if (can_move) {
            // update map
            int8_t pos[][2] = {{center_x, center_y}, {center_x, (int8_t) (center_y - 1)}, {center_x, (int8_t) (center_y - 2)}, {(int8_t) (center_x + 1), (int8_t) (center_y - 1)}};
            update_Tiles(map, pos);

            // update center
            map->moving_Piece.center_y--;
            // update rotation
            map->moving_Piece.nr_rotation++;
          }
        } else {  // we are not in an edge situation
          switch (map->moving_Piece.nr_rotation % 4) {
            case 0: {
                if (map->int_matrix[center_y - 1][center_x] > 0)
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), center_y}, {center_x, (int8_t) (center_y + 1)}, {center_x, (int8_t) (center_y - 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 1: {
                if (map->int_matrix[center_y][center_x + 1] > 0)
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x + 1), center_y}, {(int8_t) (center_x - 1), center_y}, {center_x, (int8_t) (center_y - 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 2: {
                if (map->int_matrix[center_y + 1][center_x] > 0)
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x + 1), center_y}, {center_x, (int8_t) (center_y - 1)}, {center_x, (int8_t) (center_y + 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            case 3: {
                if (map->int_matrix[center_y][center_x - 1] > 0)
                  can_move = false;
                if (can_move) {
                  // update map
                  int8_t pos[][2] = {{center_x, center_y}, {(int8_t) (center_x - 1), center_y}, {(int8_t) (center_x + 1), center_y}, {center_x, (int8_t) (center_y + 1)}};
                  update_Tiles(map, pos);

                  // update rotation
                  map->moving_Piece.nr_rotation++;
                }
              }; break;
            default: break; // error
          }
        }
      }; break;
    default: break;
  }
}

/*
    This function assign an id to the last moving piece. The id are assigned horizontally, e.g.:
    [ id1 ]
    [ id2 ] [ id2 ]
            [ id3 ] [ id3 ] [ id3 ]
    In this way, once the piece fall down it mantains his shape
*/
void save_bond(Map* map) {
  int8_t x, y, y1, y2, y3, id1, id2, id3;
  y1 = map->moving_Piece.tiles[0].y;
  y2 = -1;
  y3 = -1;
  id1 = get_from_int_buffer(&map->avaible_id);
  map->int_matrix[y1][map->moving_Piece.tiles[0].x] = id1;
  map->row_occupation[y1]++;
  for (uint8_t i = 1; i < map->moving_Piece.nr_tiles; i++) {
    y = map->moving_Piece.tiles[i].y;
    x = map->moving_Piece.tiles[i].x;
    if (y == y1) {
      map->int_matrix[y][x] = id1;
    } else if (y2 == -1) {
      y2 = y;
      id2 = get_from_int_buffer(&map->avaible_id);
      map->int_matrix[y][x] = id2;
    } else {
      if (y == y2) {
        map->int_matrix[y][x] = id2;
      } else if (y3 == -1) {
        y3 = y;
        id3 = get_from_int_buffer(&map->avaible_id);
        map->int_matrix[y][x] = id3;
      } else {
        map->int_matrix[y3][x] = id3;
      }
    }
    map->row_occupation[y]++;
  }
}

/*
    This function will make the piece of a row fall down until they found no sapce, mantaining the shape of the pieces
    It bases itself on the id of the pieces.
*/
void fall_row(Map* map, int8_t y) {
  uint16_t colour;
  int8_t j, max_so_far, previous_id = -1;
  int8_t bond[MAX_COLUMNS];
  for (int i = 0; i < MAX_COLUMNS; ++i) {
    if (map->int_matrix[y][i] != 0) {
      if (map->int_matrix[y][i] == previous_id) {
        bond[i] = bond[i - 1];
        for (j = y + 1; j <= max_so_far; ++j) {
          if (map->int_matrix[j][i] != 0)
            max_so_far = j - 1;
        }
      } else {
        bond[i] = i;
        previous_id = map->int_matrix[y][i];
        max_so_far = MAX_COLUMNS - 1;
        for (int j = y + 1; j <= max_so_far; ++j) {
          if (map->int_matrix[j][i] != 0)
            max_so_far = j - 1;
        }
      }
      if ((max_so_far > y) && ((i + 1 == MAX_COLUMNS) || (previous_id != map->int_matrix[y][i + 1]))) { // lazy evaluation otherwise error (overflow in int_matrix)
        // noInterrupts()
        for (j = bond[i]; j <= i; j++) {
          colour = map->char_matrix[y][j];
          map->int_matrix[max_so_far][j] = previous_id;
          map->char_matrix[max_so_far][j] = colour;
          draw_Tile(j, max_so_far, colour);
          map->row_occupation[max_so_far]++;

          map->int_matrix[y][j] = 0;
          map->char_matrix[y][j] = 0x0000;
          draw_Tile(j, y, 0x0000);
          map->row_occupation[y]--;
          //  UPDATE PIXELS
        }
        // interrupts()
      }
    }
  }
}

/*
    Delete all the piece on a row and return give their id to the avaible_id buffer (Because they are avaible now)
    Give points to the user.
*/
void delete_row(Map* map, int8_t y) {
  int8_t previous_id = -1;
  // noInterrupts()
  for (uint8_t i = 0; i < MAX_COLUMNS; ++i) {
    if (previous_id != map->int_matrix[y][i]) {
      previous_id = map->int_matrix[y][i];
      add_to_int_buffer(&map->avaible_id, previous_id);
    }
    map->int_matrix[y][i] = 0;
    map->char_matrix[y][i] = 0x0000;
    draw_Tile(i, y, 0x0000);
  }
  // interrupts()
  map->row_occupation[y] = 0;
  map->points += POINTS_FOR_ROW;
}

/*
    Save the moving Piece as a static Piece, check for complete row and create the new Piece. If the creation of the Piece fail it returns 1, otherwise 0
*/
uint8_t kill_Piece(Map* map) {
  // check for bond and save the piece into the matrix (when is it moving is saved as -1)
  save_bond(map);

  // check for complete row if so delete row and make upper row falling dowm. Check until nothing is moved for 1 cycle
  int8_t first, i;
  bool moved = true;
  while (moved) {
    moved = false;
    for (i = 0; i < MAX_COLUMNS; i++) {
      if (map->row_occupation[i] == 10) {
        delete_row(map, i);
        moved = true;
        first = i;
      }
    }
    if (moved) {
      for (i = first; i >= 0; i--) {
        fall_row(map, i);
      }
    }
  }

  // add the new Piece
  return new_Piece(map, (Shape) 0);
}

/*
    Given the map and a direction, it moves the moving piece to the direction provided if it can actually move amd returns 0.
    If it can not move it does nothing and returns 0, except if the direction is down. In that case it call kill_Piece() (which return 1 if the game ended, 0 otherwise)

    For every direction it watches the tiles the piece would occupy, if are empty the piece cannot move, else it can move it.
    When moving the piece in the int matrix, the final tiles have -1 id, and the lefted ones have +1 id, in this way we avoid overwriting the movement. For the char
    matrix instead we mantain the area of the movement (defined with x_min, x_max, y_min, y_max) and read in that area the id of the tiles, when is 0 we have to change it to
    no colour, when is -1 it means the piece is there and we have to paint the cell with his colours.

    The differences between this function are that update_Tiles are in the check of the space (update_Tiles does not do it) and
    move_Piece does not need an array of support, instead uses min and max values to know where to check changes of colour
*/
uint8_t move_Piece(Map* map, Direction direction) {
  bool can_move = true;
  int8_t x, y, x_max = -1, x_min = 100, y_max = -1, y_min = 100;
  uint8_t i, j, nr_tiles;
  uint16_t colour;
  nr_tiles = map->moving_Piece.nr_tiles;
  colour = map->moving_Piece.colour;
  switch (direction) {
    case 0: { // R
        // check if we can move
        for (i = 0; i < nr_tiles; ++i) {
          x = map->moving_Piece.tiles[i].x + 1;
          y = map->moving_Piece.tiles[i].y;
          if (x == MAX)
            can_move = false;
          else if (map->int_matrix[y][x] > 0)
            can_move = false;
        }
        if (can_move) {
          // noInterrupts()
          for (i = 0; i < nr_tiles; i++) {
            // ottengo coordinate della casella
            x = map->moving_Piece.tiles[i].x;
            y = map->moving_Piece.tiles[i].y;

            // muovo caselle di Piece
            map->moving_Piece.tiles[i].x++;

            // muovo valori di int_matrix (sottraggo e aggiungo per modificare i valori per non sovrascrivere)
            map->int_matrix[y][x] += 1;
            map->int_matrix[y][x + 1] -= 1;

            // per i caratteri memorizzo i bordi
            if (x > x_max)
              x_max = x;
            if (y > y_max)
              y_max = y;
            if (x < x_min)
              x_min = x;
            if (y < y_min)
              y_min = y;
          }
          for (i = x_min; i <= x_max + 1; i++) {
            for (j = y_min; j <= y_max; j++) {
              if (map->int_matrix[j][i] == -1) {
                map->char_matrix[j][i] = colour;
                draw_Tile(i, j, colour);
              }
              else if (map->int_matrix[j][i] == 0) {
                map->char_matrix[j][i] = 0x0000;
                draw_Tile(i, j, 0x0000);
              }
              // UPDATE PIXELS
            }
          }
          map->moving_Piece.center_x++;
          // interrupts()
        }
      } break;
    case 1: { // L
        // check if we can move
        for (i = 0; i < nr_tiles; ++i) {
          x = map->moving_Piece.tiles[i].x - 1;
          y = map->moving_Piece.tiles[i].y;
          if (x < 0)
            can_move = false;
          else if (map->int_matrix[y][x] > 0)
            can_move = false;
        }
        if (can_move) {
          // noInterrupts()
          for (i = 0; i < nr_tiles; i++) {
            // ottengo coordinate della casella
            x = map->moving_Piece.tiles[i].x;
            y = map->moving_Piece.tiles[i].y;

            // muovo caselle di Piece
            map->moving_Piece.tiles[i].x--;

            // muovo valori di int_matrix (sottraggo e aggiungo per modificare i valori per non sovrascrivere)
            map->int_matrix[y][x] += 1;
            map->int_matrix[y][x - 1] -= 1;

            // per i caratteri memorizzo i bordi
            if (x > x_max)
              x_max = x;
            if (y > y_max)
              y_max = y;
            if (x < x_min)
              x_min = x;
            if (y < y_min)
              y_min = y;
          }
          for (i = x_min - 1; i <= x_max; i++) {
            for (j = y_min; j <= y_max; j++) {
              if (map->int_matrix[j][i] == -1) {
                map->char_matrix[j][i] = colour;
                draw_Tile(i, j, colour);
              }
              else if (map->int_matrix[j][i] == 0) {
                map->char_matrix[j][i] = 0x0000;
                draw_Tile(i, j, 0x0000);
              }
              // UPDATE PIXELS
            }
          }
          map->moving_Piece.center_x--;
          // interrupts()
        }
      } break;
    case 2: { // D
        // check if we can move
        for (i = 0; i < nr_tiles; ++i) {
          x = map->moving_Piece.tiles[i].x;
          y = map->moving_Piece.tiles[i].y + 1;
          if (y == MAX) {
            can_move = false;
          } else if (map->int_matrix[y][x] > 0)
            can_move = false;
        }
        if (can_move) {
          // noInterrupts()
          for (i = 0; i < nr_tiles; i++) {
            // ottengo coordinate della casella
            x = map->moving_Piece.tiles[i].x;
            y = map->moving_Piece.tiles[i].y;

            // muovo caselle di Piece
            map->moving_Piece.tiles[i].y++;

            // muovo valori di int_matrix (sottraggo e aggiungo per modificare i valori per non sovrascrivere)
            map->int_matrix[y][x] += 1;
            map->int_matrix[y + 1][x] -= 1;

            // per i caratteri memorizzo i bordi
            if (x > x_max)
              x_max = x;
            if (y > y_max)
              y_max = y;
            if (x < x_min)
              x_min = x;
            if (y < y_min)
              y_min = y;
          }
          for (i = x_min; i <= x_max; i++) {
            for (j = y_min; j <= y_max + 1; j++) {
              if (map->int_matrix[j][i] == -1) {
                map->char_matrix[j][i] = colour;
                draw_Tile(i, j, colour);
              }
              else if (map->int_matrix[j][i] == 0) {
                map->char_matrix[j][i] = 0x0000;
                draw_Tile(i, j, 0x0000);
              }
              // UPDATE PIXELS
            }
          }
          map->moving_Piece.center_y++;
          map->points += POINTS_FOR_FALL;
          // interrupts()
        } else { // cant move, kill the piece
          return kill_Piece(map);
        }
      } break;
    default: break;
  }
  return 0;
}

/*
 *  The function make fall the moving Piece until it cannot fall more, the points are doubled
 */
uint8_t choke_Piece(Map* map) {
  uint32_t point = 0;
  bool ended = false;

  bool can_move = true;
  int8_t x, y, x_max = -1, x_min = 100, y_max = -1, y_min = 100;
  uint8_t i, j, nr_tiles;
  uint16_t colour;

  nr_tiles = map->moving_Piece.nr_tiles;
  colour = map->moving_Piece.colour;

  while (!ended) {
    // check if we can move
    for (i = 0; i < nr_tiles; ++i) {
      x = map->moving_Piece.tiles[i].x;
      y = map->moving_Piece.tiles[i].y + 1;
      if (y == MAX) {
        can_move = false;
      } else if (map->int_matrix[y][x] > 0)
        can_move = false;
    }
    if (can_move) {
      // noInterrupts()
      for (i = 0; i < nr_tiles; i++) {
        // ottengo coordinate della casella
        x = map->moving_Piece.tiles[i].x;
        y = map->moving_Piece.tiles[i].y;

        // muovo caselle di Piece
        map->moving_Piece.tiles[i].y++;

        // muovo valori di int_matrix (sottraggo e aggiungo per modificare i valori per non sovrascrivere)
        map->int_matrix[y][x] += 1;
        map->int_matrix[y + 1][x] -= 1;

        // per i caratteri memorizzo i bordi
        if (x > x_max)
          x_max = x;
        if (y > y_max)
          y_max = y;
        if (x < x_min)
          x_min = x;
        if (y < y_min)
          y_min = y;
      }
      map->moving_Piece.center_y++;
      point += POINTS_FOR_FALL;
      // interrupts()
    } else { // cant move, kill the piece
      ended = true;
    }
  };
  for (i = x_min; i <= x_max; i++) {
    for (j = y_min; j <= y_max + 1; j++) {
      if (map->int_matrix[j][i] == -1) {
        map->char_matrix[j][i] = colour;
        draw_Tile(i, j, colour);
      }
      else if (map->int_matrix[j][i] == 0) {
        map->char_matrix[j][i] = 0x0000;
        draw_Tile(i, j, 0x0000);
      }
      // UPDATE PIXELS
    }
  }
  map->points += (point * 2);
  return kill_Piece(map);
}

void start_game(Map* map) {
  initialize_Map(map);
  new_Piece(map, (Shape) 0);
}
/*
  void start_game(Map* map);
  uint8_t move_Piece(Map* map, Direction direction);
  void rotate_Piece(Map* map);
*/

//#endif /* __TETRIS_H__ */
