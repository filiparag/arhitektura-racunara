#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_DIM 4
#define RND_POW_MAX 2
#define INIT_CELL_MAX 5

typedef enum {
  INITIAL, PLAYING, LOST
} game_state;

typedef enum {
  UP, DOWN, LEFT, RIGHT
} direction;

typedef enum {
  ROW, COLUMN
} axis;

typedef __uint16_t val;
typedef __uint8_t dim;

typedef val game_board[BOARD_DIM][BOARD_DIM];

typedef struct {
  dim row, col;
} board_cell;
typedef struct {
  game_state state;
  game_board board;
  val score;
} game_store;

void board_print(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r) {
    for (dim c = 0; c < BOARD_DIM; ++c) {
      printf("%4i", board[r][c]);
    }
    printf("\n");
  }
}

dim index_first(direction dir, dim border) {
  if (dir == UP || dir == LEFT)
    return 0;
  else
    return BOARD_DIM - border;
}

dim index_last(direction dir, dim border) {
  if (dir == UP || dir == LEFT)
    return BOARD_DIM - border;
  else
    return 0;
}

__int8_t index_next(direction dir) {
  if (dir == UP || dir == LEFT)
    return 1;
  else
    return -1;
}

val rows_add(game_board board, direction dir) {
  val score = 0;
  const dim first = index_first(dir, 1);
  const dim last = index_last(dir, 1);
  const __int8_t next = index_next(dir);
  for (dim c = 0; c < BOARD_DIM; ++c)
    for (dim r = first; r != last; r += next)
      if (board[r][c] == board[r+next][c]) {
        board[r][c] *= 2;
        board[r+next][c] = 0;
        score += board[r][c];
      }
  return score;
}

val columns_add(game_board board, direction dir) {
  val score = 0;
  const dim first = index_first(dir, 1);
  const dim last = index_last(dir, 1);
  const __int8_t next = index_next(dir);
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = first; c != last; c += next)
      if (board[r][c] == board[r][c+next]) {
        board[r][c] *= 2;
        board[r][c+next] = 0;
        score += board[r][c];
      }
  return score;
}

void rows_move(game_board board, direction dir) {
  dim border = (dir == UP) ? 0 : 1;
  const dim first = index_first(dir, border);
  const dim last = index_last(dir, 0);
  const __int8_t next = index_next(dir);
  for (dim c = 0; c < BOARD_DIM; ++c)
    for (dim r = first; r != last; r += next) {
      __int8_t r_next = r + next;
      while (board[r][c] == 0 && r_next + border != last) {
        board[r][c] = board[r_next][c];
        board[r_next][c] = 0;
        r_next += next;
      }
    }
}

void columns_move(game_board board, direction dir) {
  dim border = (dir == LEFT) ? 0 : 1;
  const dim first = index_first(dir, border);
  const dim last = index_last(dir, 0);
  const __int8_t next = index_next(dir);
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = first; c != last; c += next) {
      __int8_t c_next = c + next;
      while (board[r][c] == 0 && c_next + border != last) {
        board[r][c] = board[r][c_next];
        board[r][c_next] = 0;
        c_next += next;
      }
    }
}

val inline random_value() {
  return 1 << (rand() % RND_POW_MAX + 1);
}

dim inline random_cell(dim count) {
  return rand() % count;
}

bool board_insert(game_board board) {
  board_cell empty[BOARD_DIM * BOARD_DIM];
  dim count = 0;
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      if (board[r][c] == 0) {
        empty[count] = (board_cell){.row = r, .col = c};
        ++count;
      }
  if (count > 0) {
    const dim cell = random_cell(count);
    board[empty[cell].row][empty[cell].col] = random_value();
    return true;
  } else
    return false;
}

void game_initialize(game_store *game) {
  const dim count = random_cell(INIT_CELL_MAX) + 1;
  for (dim c = 0; c < count; ++c)
    board_insert(game->board);
  game->state = PLAYING;
}

void game_action(game_store *game, direction dir) {
  if (game->state != PLAYING)
    return;
  if (dir == LEFT || dir == RIGHT) {
    columns_move(game->board, dir);
    game->score += columns_add(game->board, dir);
    columns_move(game->board, dir);
  } else {
    rows_move(game->board, dir);
    game->score += rows_add(game->board, dir);
    rows_move(game->board, dir);
  }
  if (!board_insert(game->board)) {
    game->state = LOST;
  }
}

int main2048() {
  srand(time(NULL));
  game_store game = {INITIAL};
  game_initialize(&game);
  char input;
  while (game.state != LOST) {
    printf("\x1B[1;1H\x1B[2J");
    printf("Score: %9i\n\n", game.score);
    board_print(game.board);
    printf("\n:");
    scanf("%c", &input);
    switch (input) {
      case 'q':
        return 0;
      case 'u': {
        game_action(&game, UP);
        break;
      }
      case 'd': {
        game_action(&game, DOWN);
        break;
      }
      case 'l': {
        game_action(&game, LEFT);
        break;
      }
      case 'r': {
        game_action(&game, RIGHT);
        break;
      }
      default:
        break;
    }
  }
  return 0;
}
