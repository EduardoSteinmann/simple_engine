#ifndef _CONST
#define _CONST

#ifdef INFINITY

#undef INFINITY
#define INFINITY 1000000000

#else

#define INFINITY 1000000000

#endif // INFINITY

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define BOARD_WIDTH 800

#define ROWS 8
#define COLS 8

#define TILE_SIZE (BOARD_WIDTH / COLS)

#define MAX_NUM_OF_MOVES 27
#define MAX_PIECES_ON_ONE_SIDE 16

#define EMPTY_MOVE (Move){ { -1, -1 }, { -1, -1 } }
#define EMPTY_MOVES (Moves){ { EMPTY_MOVE } }

#define WHITE -1
#define BLACK 1
#define NO_COLOR 0

#define EMPTY -1
#define PAWN 100
#define PAWN_WITH_DOUBLE_JUMP 101
#define KNIGHT 300
#define BISHOP 325
#define ROOK 500
#define QUEEN 900
#define KING 20000

#define REGULAR_MOVE 2
#define CAPTURE 3
#define KING_SIDE_CASTLING 4
#define QUEEN_SIDE_CASTLING 5
#define PAWN_PROMOTION 20

#define SEARCH_DEPTH 4

#define PVP_MODE 1
#define AI_MODE 2
#define QUIT_MODE 6
#define START_SCREEN 4
#define COLOR_SELECTION 5
#define CHECKMATE 10
#define STALEMATE 11
#define BACK_TO_START 9

#define init_piece(value, color, x, y) ((Piece){ false, value, color, (Coord) { x, y } })
#define init_empty() (Piece){ false, EMPTY, NO_COLOR }

#endif