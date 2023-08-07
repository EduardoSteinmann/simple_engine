#ifndef _GAME
#define _GAME

#include "GUI.h"

const int start_screen(const SDL_Event* const event, const Coord* const mouse);

const int color_selection(SDL_Renderer* const renderer, const SDL_Event* const event, const Coord* const mouse, int* const player_color, int* const AI_color);

const bool is_PVP_clicked(const Coord* const mouse);

const bool is_AI_clicked(const Coord* const mouse);

const bool is_white_clicked(const Coord* const mouse);

const bool is_black_clicked(const Coord* const mouse);

const int PVP_mode(Piece board[ROWS][COLS], const SDL_Event* const event, Coord* const mouse, SDL_Renderer* const renderer, int* const turn);

const int player_move(Piece board[ROWS][COLS], const SDL_Event* const event, Coord* const mouse, SDL_Renderer* const renderer, int* const turn, const int player_color);

const int AI_move(Piece board[ROWS][COLS], SDL_Renderer* const renderer, const int AI_color, int* const turn);

const int pawn_promotion(Piece board[ROWS][COLS], const Piece* const pawn, const SDL_Event* const event, Coord* const mouse, SDL_Renderer* const renderer, bool* const is_promoting);

const int get_promotion_y_coord(const int pawn_y, const int color, const int num_on_menu);

const bool is_promotion_button_clicked(const Coord* const mouse, const int x_coord, const int y_coord);

const int checkmate_screen(SDL_Renderer* const renderer, const SDL_Event* const event, const Coord* const mouse, const int* const turn);

const int stalemate_screen(SDL_Renderer* const renderer, const SDL_Event* const event, const Coord* const mouse);

const bool is_back_to_start_clicked(const Coord* const mouse);

const int game_loop();

#endif // !_GAME