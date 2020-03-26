#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"

void board_print(
	game_board board
);

dim index_first(
	const direction dir, const dim border
);

dim index_last(
	const direction dir, const dim border
);

sdim index_next(
	const direction dir
);

void delta_move(
	board_change* change, const val value,
  const dim ro, const dim co,
  const dim rd, const dim cd,
  const bool merge
);

void delta_add(
	board_change* change, const val value,
  const dim row, const dim col
);

void delta_insert(
	board_change* change, const val value,
  const dim row, const dim col
);

void delta_clear(
	board_change* change
);

val rows_add(
	game_board board, const direction dir,
  board_change* change
);

val columns_add(
	game_board board, const direction dir,
  board_change* change
);

bool rows_move(
	game_board board, const direction dir,
  board_change* change
);

bool columns_move(
	game_board board, const direction dir,
  board_change* change
);

val random_value();

dim random_cell(
	const dim count
);

bool board_insert(
	game_board board, board_change* change
);

void board_clear(
	game_board board
);

val board_max(
	game_board board
);

bool board_out_of_moves(
	game_board board
);

void game_initialize(
	game_store *game
);

void game_action(
	game_store *game, const direction dir
);

void game_play_console();