#ifndef _AI_CALCS
#define _AI_CALCS
#include "legal_move_calculation.h"

const int test_move(Piece board[ROWS][COLS], const Move move);

const int eval_board(Piece board[ROWS][COLS]);

const int max(const int l_value, const int r_value);

const int min(const int l_value, const int r_value);

const Move generate_move(Piece board[ROWS][COLS], const int color, const int search_depth);

const int minimax(Piece board[ROWS][COLS], const int color, const int search_depth, int max_assured_score, int min_assured_score);

#endif