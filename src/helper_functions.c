#include "helper_functions.h"

const int get_opposite_color(const int color)
{
	if (color == WHITE)
	{
		return BLACK;
	}
	else if (color == BLACK)
	{
		return WHITE;
	}
	else
	{
		return 0;
	}
}

Piece* get_king(Piece board[ROWS][COLS], const int color)
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (get_piece_value(&board[row][col]) == KING && board[row][col].color == color)
			{
				return &board[row][col];
			}
		}
	}
	return NULL;
}

void init_new_board(Piece board[ROWS][COLS])
{
	const int BLACK_PIECE_ROW = 0;
	const int WHITE_PIECE_ROW = 7;
	const int BLACK_PAWN_ROW = 1;
	const int WHITE_PAWN_ROW = 6;

	board[BLACK_PIECE_ROW][0] = init_piece(ROOK, BLACK, 0, BLACK_PIECE_ROW);
	board[BLACK_PIECE_ROW][1] = init_piece(KNIGHT, BLACK, 1, BLACK_PIECE_ROW);
	board[BLACK_PIECE_ROW][2] = init_piece(BISHOP, BLACK, 2, BLACK_PIECE_ROW);
	board[BLACK_PIECE_ROW][3] = init_piece(QUEEN, BLACK, 3, BLACK_PIECE_ROW);
	board[BLACK_PIECE_ROW][4] = init_piece(KING, BLACK, 4, BLACK_PIECE_ROW);
	board[BLACK_PIECE_ROW][5] = init_piece(BISHOP, BLACK, 5, BLACK_PIECE_ROW);
	board[BLACK_PIECE_ROW][6] = init_piece(KNIGHT, BLACK, 6, BLACK_PIECE_ROW);
	board[BLACK_PIECE_ROW][7] = init_piece(ROOK, BLACK, 7, BLACK_PIECE_ROW);

	for (int col = 0; col < COLS; col++)
	{
		board[BLACK_PAWN_ROW][col] = init_piece(PAWN_THAT_CAN_EN_PASSANT, BLACK, col, BLACK_PAWN_ROW);
	}

	for (int row = 2; row < 6; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			board[row][col] = init_empty();
		}
	}

	for (int col = 0; col < COLS; col++)
	{
		board[WHITE_PAWN_ROW][col] = init_piece(PAWN_THAT_CAN_EN_PASSANT, WHITE, col, WHITE_PAWN_ROW);
	}

	board[WHITE_PIECE_ROW][0] = init_piece(ROOK, WHITE, 0, WHITE_PIECE_ROW);
	board[WHITE_PIECE_ROW][1] = init_piece(KNIGHT, WHITE, 1, WHITE_PIECE_ROW);
	board[WHITE_PIECE_ROW][2] = init_piece(BISHOP, WHITE, 2, WHITE_PIECE_ROW);
	board[WHITE_PIECE_ROW][3] = init_piece(QUEEN, WHITE, 3, WHITE_PIECE_ROW);
	board[WHITE_PIECE_ROW][4] = init_piece(KING, WHITE, 4, WHITE_PIECE_ROW);
	board[WHITE_PIECE_ROW][5] = init_piece(BISHOP, WHITE, 5, WHITE_PIECE_ROW);
	board[WHITE_PIECE_ROW][6] = init_piece(KNIGHT, WHITE, 6, WHITE_PIECE_ROW);
	board[WHITE_PIECE_ROW][7] = init_piece(ROOK, WHITE, 7, WHITE_PIECE_ROW);
}

const bool pawn_double_jumped(const Piece* const pawn)
{
	return pawn->value == PAWN_WITH_DOUBLE_JUMP;
}

const bool has_move(const Piece* const piece, const Move move)
{
	for (int i = 0; i < piece->moves.length; i++)
	{
		if (equal_moves(piece->moves.moves[i], move))
		{
			return true;
		}
	}
	return false;
}

const bool has_no_moves(const Piece* const piece)
{
	return !piece->moves.length;
}

Coord add_coords(const Coord coord1, const Coord coord2)
{
	Coord new_coord = { coord1.x + coord2.x, coord1.y + coord2.y };
	return new_coord;
}

const bool is_valid_coord(const Coord coord)
{
	return coord.x >= 0 && coord.x < COLS && coord.y >= 0 && coord.y < ROWS;
}

const bool is_valid_row_and_col(const int col, const int row)
{
	return col >= 0 && col < COLS && row >= 0 && row < ROWS;
}

const bool is_empty(Piece board[ROWS][COLS], const Coord coord)
{
	return get_piece_value(&board[coord.y][coord.x]) == EMPTY;
}

const bool is_occupied_by_enemy_piece(Piece board[ROWS][COLS], const int current_color, const Coord coord)
{
	return !is_empty(board, coord) && board[coord.y][coord.x].color != current_color;
}

const bool is_occupied_by_allied_piece(Piece board[ROWS][COLS], const int current_color, const Coord coord)
{
	return !is_empty(board, coord) && board[coord.y][coord.x].color == current_color;
}

const bool equal_coords(const Coord coord1, const Coord coord2)
{
	return coord1.x == coord2.x && coord1.y == coord2.y;
}

const bool check_if_moves_is_empty(Move moves[MAX_NUM_OF_MOVES])
{
	return equal_moves(moves[0], EMPTY_MOVE);
}

const bool equal_moves(const Move move1, const Move move2)
{
	return equal_coords(move1.initial, move2.initial) && equal_coords(move1.final, move2.final);
}

const bool piece_contains_coord_in_moves(const Piece* const piece, Coord coord)
{
	for (int i = 0; i < piece->moves.length; i++)
	{
		if (equal_coords(piece->moves.moves[i].final, coord))
		{
			return true;
		}
	}
	return false;
}

const int get_piece_value(const Piece* const piece)
{
	if (piece->value == PAWN_WITH_DOUBLE_JUMP || piece->value == PAWN_B_EN_PASSANT
		|| piece->value == PAWN_L_EN_PASSANT || piece->value == PAWN_R_EN_PASSANT
		|| piece->value == PAWN_THAT_CAN_EN_PASSANT || piece->value == PAWN_THAT_CAN_EN_PASSANT_R
		|| piece->value == PAWN_THAT_CAN_EN_PASSANT_L || piece->value == PAWN_THAT_CAN_EN_PASSANT_L_BUT_NOT_R 
		|| piece->value == PAWN_THAT_CAN_EN_PASSANT_R_BUT_NOT_L || piece->value == PAWN_L_EN_PASSANT_BUT_NOT_R
		|| piece->value == PAWN_R_EN_PASSANT_BUT_NOT_L)
	{
		return PAWN;
	}
	return piece->value;
}

const int get_piece_value_on_board(const int piece_value, const int piece_color, const Coord coords)
{
	const int PAWN_TABLE[ROWS][COLS] =
	{
		{0,  0,  0,  0,  0,  0,  0,  0},
		{50, 50, 50, 50, 50, 50, 50, 50},
		{10, 10, 20, 30, 30, 20, 10, 10},
		{5,  5, 10, 45, 45, 10,  5,  5},
		{0,  0,  0, 20, 20,  0,  0,  0},
		{5, -5,-10,  0,  0,-10, -5,  5},
		{5, 10, 10,-20,-20, 10, 10,  5},
		{0,  0,  0,  0,  0,  0,  0,  0}
	};

	const int BISHOP_TABLE[ROWS][COLS] =
	{
		{-20,-10,-10,-10,-10,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5, 10, 10,  5,  0,-10},
		{-10,  5,  5, 10, 10,  5,  5,-10},
		{-10,  0, 10, 10, 10, 10,  0,-10},
		{-10, 10, 10, 10, 10, 10, 10,-10},
		{-10,  5,  0,  0,  0,  0,  5,-10},
		{-20,-10,-10,-10,-10,-10,-10,-20},
	};

	const int KNIGHT_TABLE[ROWS][COLS] =
	{
		{-50,-40,-30,-30,-30,-30,-40,-50},
		{-40,-20,  0,  0,  0,  0,-20,-40},
		{-30,  0, 10, 15, 15, 10,  0,-30},
		{-30,  5, 15, 20, 20, 15,  5,-30},
		{-30,  0, 15, 20, 20, 15,  0,-30},
		{-30,  5, 10, 15, 15, 10,  5,-30},
		{-40,-20,  0,  5,  5,  0,-20,-40},
		{-50,-40,-30,-30,-30,-30,-40,-50},
	};

	const int QUEEN_TABLE[ROWS][COLS] =
	{
		{-20,-10,-10, -5, -5,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5,  5,  5,  5,  0,-10},
		{-5,  0,  5,  5,  5,  5,  0, -5},
		{0,  0,  5,  5,  5,  5,  0, -5},
		{-10,  5,  5,  5,  5,  5,  0,-10},
		{-10,  0,  5,  0,  0,  0,  0,-10},
		{-20,-10,-10, -5, -5,-10,-10,-20}
	};

	const int KING_TABLE[ROWS][COLS] =
	{
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-20,-30,-30,-40,-40,-30,-30,-20},
		{-10,-20,-20,-20,-20,-20,-20,-10},
		{20, 20,  0,  0,  0,  0, 20, 20},
		{20, 30, 10,  0,  0, 10, 30, 20 },
	};

	const int ROOK_TABLE[ROWS][COLS] =
	{
		{0,  0,  0,  0,  0,  0,  0,  0},
		{5, 10, 10, 10, 10, 10, 10,  5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{0,  0,  0,  5,  5,  0,  0,  0}
	};

	const int adjustment = piece_color == WHITE ? 0 : 7;

	int piece_value_on_board = piece_value;

	if (piece_value_on_board == PAWN)
	{
		piece_value_on_board += PAWN_TABLE[adjustment - (coords.y * piece_color)][coords.x];
	}
	else if (piece_value_on_board == KNIGHT)
	{
		piece_value_on_board += KNIGHT_TABLE[adjustment - (coords.y * piece_color)][coords.x];
	}
	else if (piece_value_on_board == BISHOP)
	{
		piece_value_on_board += BISHOP_TABLE[adjustment - (coords.y * piece_color)][coords.x];
	}
	else if (piece_value_on_board == ROOK)
	{
		piece_value_on_board += ROOK_TABLE[adjustment - (coords.y * piece_color)][coords.x];
	}
	else if (piece_value_on_board == QUEEN)
	{
		piece_value_on_board += QUEEN_TABLE[adjustment - (coords.y * piece_color)][coords.x];
	}
	else if (piece_value_on_board == KING)
	{
		piece_value_on_board += KING_TABLE[adjustment - (coords.y * piece_color)][coords.x];
	}

	return piece_value_on_board;
}

void clone_board(Piece board[ROWS][COLS], Piece dest[ROWS][COLS])
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			dest[row][col] = board[row][col];
		}
	}
}

void set_pawn_as_double_jumped(Piece* const pawn)
{
	pawn->value = PAWN_WITH_DOUBLE_JUMP;
}

const bool can_en_passant_right(Piece* const pawn)
{
	return pawn->value == PAWN_THAT_CAN_EN_PASSANT_R || pawn->value == PAWN_THAT_CAN_EN_PASSANT || pawn->value == PAWN_WITH_DOUBLE_JUMP 
		|| pawn->value == PAWN_R_EN_PASSANT || pawn->value == PAWN_B_EN_PASSANT || pawn->value == PAWN_THAT_CAN_EN_PASSANT_R_BUT_NOT_L 
		|| pawn->value == PAWN_R_EN_PASSANT_BUT_NOT_L;
}

const bool can_en_passant_left(Piece* const pawn)
{
	return pawn->value == PAWN_THAT_CAN_EN_PASSANT_L || pawn->value == PAWN_THAT_CAN_EN_PASSANT || pawn->value == PAWN_WITH_DOUBLE_JUMP 
		|| pawn->value == PAWN_L_EN_PASSANT || pawn->value == PAWN_B_EN_PASSANT || pawn->value == PAWN_THAT_CAN_EN_PASSANT_L_BUT_NOT_R 
		|| pawn->value == PAWN_L_EN_PASSANT_BUT_NOT_R;
}

void set_pawn_en_passant_right(Piece* const pawn)
{
	if (pawn->value == PAWN_THAT_CAN_EN_PASSANT_R_BUT_NOT_L || pawn->value == PAWN_R_EN_PASSANT_BUT_NOT_L)
	{
		pawn->value = PAWN_R_EN_PASSANT_BUT_NOT_L;
		return;
	}

	pawn->value = PAWN_R_EN_PASSANT;
}

void set_pawn_en_passant_left(Piece* const pawn)
{
	if (pawn->value == PAWN_R_EN_PASSANT)
	{
		pawn->value = PAWN_B_EN_PASSANT;
		return;
	}

	if (pawn->value == PAWN_THAT_CAN_EN_PASSANT_L_BUT_NOT_R || pawn->value == PAWN_L_EN_PASSANT_BUT_NOT_R)
	{
		pawn->value = PAWN_L_EN_PASSANT_BUT_NOT_R;
		return;
	}

	pawn->value = PAWN_L_EN_PASSANT;
}

const bool pawn_can_perform_right_en_passant(const Piece* const pawn)
{
	return pawn->value == PAWN_R_EN_PASSANT || pawn->value == PAWN_R_EN_PASSANT_BUT_NOT_L || pawn->value == PAWN_B_EN_PASSANT;
}

const bool pawn_can_perform_left_en_passant(const Piece* const pawn)
{
	return pawn->value == PAWN_L_EN_PASSANT || pawn->value == PAWN_L_EN_PASSANT_BUT_NOT_R || pawn->value == PAWN_B_EN_PASSANT;
}

const bool pawn_can_perform_both_en_passant(const Piece* const pawn)
{
	return pawn->value == PAWN_B_EN_PASSANT;
}