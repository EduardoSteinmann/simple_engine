#ifndef _GUI
#define _GUI
#include "AI_calculation.h"

SDL_Surface* get_piece_image(const int piece_value, const int color);

const int display_start_screen(SDL_Renderer* const renderer);

const int display_color_selection(SDL_Renderer* const renderer);

const int display_board(SDL_Renderer* const renderer);

const int display_pieces(SDL_Renderer* const renderer, Piece board[ROWS][COLS], const Piece* dragging_piece);

const int display_all_legal_moves(SDL_Renderer* const renderer, Piece board[ROWS][COLS], const Piece* const piece, const int current_player);

const int display_dragged_piece(SDL_Renderer* const renderer, const Coord mouse, const int piece_value, const int color);

const int display_promotion_button(SDL_Renderer* const renderer, const int x_coord, const int y_coord, const int piece_value, const int color);

const int display_checkmate_screen(SDL_Renderer* const renderer, const int winner);

const int display_stalemate_screen(SDL_Renderer* const renderer);

#endif