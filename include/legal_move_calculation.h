#ifndef _CALCULATION
#define _CALCULATION
#include "helper_functions.h"

const int make_move(Piece board[ROWS][COLS], Move move, const int current_player, const bool is_calculating);

void perform_en_passant_capture_if_that_is_the_move(Piece board[ROWS][COLS], const Move move, const Piece* const pawn);

const int move_rook_if_castling(Piece board[ROWS][COLS], Piece* king, const Move move, int move_code);

void castling_calculation(Piece* king, Piece board[ROWS][COLS], const bool in_check);

void remove_en_passant_if_not_done_immediately(Piece board[ROWS][COLS], const int color);

const bool is_king_in_check(const Piece* king, Piece board[ROWS][COLS]);

const bool would_put_king_in_check(Piece board[ROWS][COLS], const Move move, const int colir);

const bool would_remove_check(Piece board[ROWS][COLS], const Move move, const int color);

const bool in_check_mate(Piece board[ROWS][COLS], const bool in_check, const int color);

const bool in_stale_mate(Piece board[ROWS][COLS], const bool in_check, const int color);

void calculate_all_moves_for_a_side(Piece board[ROWS][COLS], Piece* king, const bool in_check, const int color);

void filter_moves(Piece board[ROWS][COLS], Piece* piece, const bool in_check);

void calculate_legal_moves(Piece board[ROWS][COLS], Piece* piece);

void calculate_pawn_moves(Piece board[ROWS][COLS], Piece* pawn);

void calculate_knight_moves(Piece board[ROWS][COLS], Piece* knight);

void calculate_bishop_moves(Piece board[ROWS][COLS], Piece* bishop);

void calculate_rook_moves(Piece board[ROWS][COLS], Piece* rook);

void calculate_queen_moves(Piece board[ROWS][COLS], Piece* queen);

void calculate_king_moves(Piece board[ROWS][COLS], Piece* king);

#endif // CALCULATION
