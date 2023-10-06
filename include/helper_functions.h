#ifndef _HELPERS
#define _HELPERS
#include "structs.h"

const int get_opposite_color(const int color);

const int get_piece_value(const Piece* const piece);

const bool pawn_double_jumped(const Piece* const pawn);

Piece* get_king(Piece board[ROWS][COLS], const int color);

Coord add_coords(const Coord coord1, const Coord coord2);

void init_new_board(Piece board[ROWS][COLS]);

const bool has_move(const Piece* const piece, const Move move);

const bool has_no_moves(const Piece* const piece);

const bool is_valid_coord(const Coord coord);

const bool is_valid_row_and_col(const int col, const int row);

const bool is_occupied_by_allied_piece(Piece board[ROWS][COLS], const int current_color, const Coord coord);

const bool is_occupied_by_enemy_piece(Piece board[ROWS][COLS], const int current_color, const Coord coord);

const bool is_empty(Piece board[ROWS][COLS], const Coord coord);

const bool equal_moves(const Move move1, const Move move2);

const bool equal_coords(const Coord coord1, const Coord coord2);

const bool piece_contains_coord_in_moves(const Piece* const piece, Coord coord);

const int get_piece_value_on_board(const int piece_value, const int piece_color, const Coord coords);

void clone_board(Piece board[ROWS][COLS], Piece dest[ROWS][COLS]);

void set_pawn_as_double_jumped(Piece* const pawn);

#endif // _HELPERS