#include "legal_move_calculation.h"

const int make_move(Piece board[ROWS][COLS], Move move, const int current_player, const bool is_calculating)
{
	int move_code = 0;

	Piece* piece = &board[move.initial.y][move.initial.x];

	if (get_piece_value(piece) == EMPTY || !is_valid_coord(move.final) || piece->color != current_player || !has_move(piece, move))
	{
		return move_code;
	}

	const bool is_pawn = get_piece_value(piece) == PAWN;

	move_code = REGULAR_MOVE;

	if (is_pawn)
	{
		if (move.final.y == move.initial.y + (2 * current_player))
		{
			set_pawn_as_double_jumped(piece);
		}
		else
		{
			perform_en_passant_capture_if_that_is_the_move(board, move, piece);
		}
	}
	else if (get_piece_value(piece) == KING)
	{
		move_code = move_rook_if_castling(board, piece, move, move_code);
	}

	if (is_occupied_by_enemy_piece(board, current_player, move.final))
	{
		move_code = CAPTURE;
	}

	board[move.final.y][move.final.x] = *piece;

	piece = &board[move.final.y][move.final.x];

	board[move.initial.y][move.initial.x] = init_empty();

	piece->moved = true;

	piece->coords = move.final;

	if (is_pawn && ((piece->color == WHITE && piece->coords.y == 0) || (piece->color == BLACK && piece->coords.y == 7)))
	{
		move_code = PAWN_PROMOTION;
	}

	if (!is_calculating)
	{
		remove_en_passant_if_not_done_immediately(board, get_opposite_color(current_player));
	}

	return move_code;
}

void perform_en_passant_capture_if_that_is_the_move(Piece board[ROWS][COLS], const Move move, const Piece* const pawn)
{
	const Move left_capture = { pawn->coords, { pawn->coords.x - 1, pawn->coords.y + pawn->color } };
	const Move right_capture = { pawn->coords, { pawn->coords.x + 1, pawn->coords.y + pawn->color } };

	if (equal_moves(move, right_capture) && pawn_double_jumped(&board[right_capture.initial.y][right_capture.final.x]))
	{
		board[right_capture.initial.y][right_capture.final.x] = init_empty();
	}
	else if (equal_moves(move, left_capture) && pawn_double_jumped(&board[left_capture.initial.y][left_capture.final.x]))
	{
		board[left_capture.initial.y][left_capture.final.x] = init_empty();
	}
}

void calculate_all_moves_for_a_side(Piece board[ROWS][COLS], Piece* king, const bool in_check, const int color)
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (!is_occupied_by_allied_piece(board, color, (Coord) { col, row }))
			{
				continue;
			}

			Piece* piece = &board[row][col];

			calculate_legal_moves(board, piece);

			filter_moves(board, piece, in_check);
		}
	}

	castling_calculation(king, board, in_check);
}

void filter_moves(Piece board[ROWS][COLS], Piece* piece, const bool in_check)
{
	Moves filtered_moves = { 0 };
	for (int move_index = 0; move_index < piece->moves.length; move_index++)
	{
		Move move = piece->moves.moves[move_index];
		if ((in_check && !would_remove_check(board, move, piece->color)) || (!in_check && would_put_king_in_check(board, move, piece->color)))
		{
			continue;
		}
		filtered_moves.moves[filtered_moves.length++] = move;
	}
	piece->moves = filtered_moves;
}

const int move_rook_if_castling(Piece board[ROWS][COLS], Piece* king, const Move move, int move_code)
{
	const int rook_row = king->color == WHITE ? 7 : 0; 

	if (move.final.y == rook_row && move.initial.x == 4)
	{
		if (piece_contains_coord_in_moves(king, (Coord) { 6, rook_row }) && move.final.x == 6)
		{
			Piece* rook = &board[rook_row][7];
			rook->moved = true;
			rook->coords = (Coord){ 5, rook_row };
			board[rook_row][5] = *rook;

			board[rook_row][7] = init_empty();

			move_code = KING_SIDE_CASTLING;
		}
		else if (piece_contains_coord_in_moves(king, (Coord) { 2, rook_row }) && move.final.x == 2)
		{
			Piece* rook = &board[rook_row][0];
			rook->moved = true;
			rook->coords = (Coord){ 3, rook_row };
			board[rook_row][3] = *rook;

			board[rook_row][0] = init_empty();

			move_code = QUEEN_SIDE_CASTLING;
		}
	}

	return move_code;
}

void castling_calculation(Piece* king, Piece board[ROWS][COLS], const bool in_check)
{
	if (king->moved || in_check)
	{
		return;
	}

	const int color = king->color;

	const int rook_row = color == WHITE ? 7 : 0;

	Piece rook = board[rook_row][0];

	if (get_piece_value(&rook) == ROOK && !rook.moved && rook.color == king->color)
	{
		bool queen_side_castling = true;

		for (int new_king_col = king->coords.x; new_king_col > 2;)
		{
			new_king_col--;

			const Coord coord_king_moves_to = { new_king_col, rook_row };

			if (!is_empty(board, coord_king_moves_to) || would_put_king_in_check(board, (Move) { king->coords, coord_king_moves_to }, king->color))
			{
				queen_side_castling = false;
				break;
			}
		}

		if (queen_side_castling)
		{
			king->moves.moves[king->moves.length++] = (Move) { king->coords, (Coord) { 2, rook_row } };
		}
	}

	rook = board[rook_row][rook_row];

	if (get_piece_value(&rook) == ROOK && !rook.moved && rook.color == king->color)
	{
		bool king_side_castling = true;

		for (int new_king_col = king->coords.x; new_king_col < 6;)
		{
			new_king_col++;

			const Coord coord_king_moves_to = { new_king_col, rook_row };

			if (!is_empty(board, coord_king_moves_to) || would_put_king_in_check(board, (Move) { king->coords, coord_king_moves_to }, king->color))
			{
				king_side_castling = false;
				break;
			}
		}

		if (king_side_castling)
		{
			king->moves.moves[king->moves.length++] = (Move){ king->coords, (Coord) { 6, rook_row } };
		}
	}
}

void remove_en_passant_if_not_done_immediately(Piece board[ROWS][COLS], const int color)
{
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (board[row][col].color != color || get_piece_value(&board[row][col]) != PAWN)
			{
				continue;
			}

			Piece* const pawn = &board[row][col];

			pawn->value = PAWN;
		}
	}
}

void calculate_legal_moves(Piece board[ROWS][COLS], Piece* piece)
{
	piece->moves.length = 0;

	if (get_piece_value(piece) == PAWN)
	{
		calculate_pawn_moves(board, piece);
	}
	else if (get_piece_value(piece) == KNIGHT)
	{
		calculate_knight_moves(board, piece);
	}
	else if (get_piece_value(piece) == BISHOP)
	{
		calculate_bishop_moves(board, piece);
	}
	else if (get_piece_value(piece) == ROOK)
	{
		calculate_rook_moves(board, piece);
	}
	else if (get_piece_value(piece) == QUEEN)
	{
		calculate_queen_moves(board, piece);
	}
	else if (get_piece_value(piece) == KING)
	{
		calculate_king_moves(board, piece);
	}
}

const bool is_king_in_check(const Piece* king, Piece board[ROWS][COLS])
{
	bool in_check = false;
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			const Coord current_coord = { col, row };
			if (is_occupied_by_enemy_piece(board, king->color, current_coord))
			{
				calculate_legal_moves(board, &board[row][col]);
				if (piece_contains_coord_in_moves(&board[row][col], king->coords))
				{
					in_check = true;
				}
			}
		}
	}
	return in_check;
}

const bool would_put_king_in_check(Piece board[ROWS][COLS], const Move move, const int color)
{
	bool in_check = false;
	Piece copy_board[ROWS][COLS];
	clone_board(board, copy_board);
	
	if (make_move(copy_board, move, color, true))
	{
		in_check = is_king_in_check(get_king(copy_board, color), copy_board);
	}

	return in_check;
}

const bool would_remove_check(Piece board[ROWS][COLS], const Move move, const int color)
{
	bool in_check = true;
	Piece copy_board[ROWS][COLS];
	clone_board(board, copy_board);

	if (make_move(copy_board, move, color, true))
	{
		in_check = is_king_in_check(get_king(copy_board, color), copy_board);
	}

	return !in_check;
}

const bool in_check_mate(Piece board[ROWS][COLS], const bool in_check, const int color)
{
	if (!in_check)
	{
		return false;
	}

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (is_occupied_by_allied_piece(board, color, (Coord) { col, row }) && board[row][col].moves.length != 0)
			{
				return false;
			}
		}
	}

	return true;
}

const bool in_stale_mate(Piece board[ROWS][COLS], const bool in_check, const int color)
{
	if (in_check)
	{
		return false;
	}

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			if (is_occupied_by_allied_piece(board, color, (Coord) { col, row }) && board[row][col].moves.length != 0)
			{
				return false;
			}
		}
	}

	return true;
}

void calculate_pawn_moves(Piece board[ROWS][COLS], Piece* pawn)
{
	int last_move_index = 0;

	const Coord initial_pawn_jump = { pawn->coords.x, pawn->coords.y + 2 * pawn->color };
	const Coord regular_pawn_jump = { pawn->coords.x, pawn->coords.y + pawn->color };
	const Coord left_capture = { pawn->coords.x - 1, pawn->coords.y + pawn->color };
	const Coord right_capture = { pawn->coords.x + 1, pawn->coords.y + pawn->color };

	const int en_passant_row = pawn->color == WHITE ? 3 : 4;

	if (!pawn->moved && is_valid_coord(initial_pawn_jump) 
		&& is_empty(board, (Coord){ pawn->coords.x, pawn->coords.y + pawn->color })
		&& is_empty(board, initial_pawn_jump))
	{
		pawn->moves.moves[last_move_index++] = (Move){ pawn->coords, initial_pawn_jump };
	}
	else if (pawn->coords.y == en_passant_row)
	{
		const Piece* const piece_to_the_left = &board[en_passant_row][pawn->coords.x - 1];
		const Piece* const piece_to_the_right = &board[en_passant_row][pawn->coords.x + 1];

		if (get_piece_value(piece_to_the_right) == PAWN && piece_to_the_left->color != pawn->color && pawn_double_jumped(piece_to_the_right))
		{
			pawn->moves.moves[last_move_index++] = (Move){ pawn->coords, right_capture };
		}

		if (get_piece_value(piece_to_the_left) == PAWN && piece_to_the_right->color != pawn->color && pawn_double_jumped(piece_to_the_left))
		{
			pawn->moves.moves[last_move_index++] = (Move){ pawn->coords, left_capture };
		}
	}
	
	if (is_valid_coord(regular_pawn_jump) && is_empty(board, regular_pawn_jump))
	{
		pawn->moves.moves[last_move_index++] = (Move){ pawn->coords, regular_pawn_jump };
	}

	if (is_valid_coord(left_capture) && is_occupied_by_enemy_piece(board, pawn->color, left_capture))
	{
		pawn->moves.moves[last_move_index++] = (Move){ pawn->coords, left_capture };
	}

	if (is_valid_coord(right_capture) && is_occupied_by_enemy_piece(board, pawn->color, right_capture))
	{
		pawn->moves.moves[last_move_index++] = (Move){ pawn->coords, right_capture };
	}

	pawn->moves.length = last_move_index;
}

void calculate_knight_moves(Piece board[ROWS][COLS], Piece* knight)
{
	int last_move_index = 0;

	Coord moves[] =
	{
		{ 2, 1 },
		{ 2, -1 },
		{ -2, 1 },
		{ -2, -1 },
		{ 1, 2 },
		{ -1, 2 },
		{ 1, -2 },
		{ -1, -2 }
	};

	for (int i = 0; i < sizeof(moves) / sizeof(Coord); i++)
	{
		Coord move = moves[i];

		Coord new_coords = add_coords(knight->coords, move);

		if (is_valid_coord(new_coords) && (is_empty(board, new_coords) || board[new_coords.y][new_coords.x].color != knight->color))
		{
			knight->moves.moves[last_move_index++] = (Move){ knight->coords, new_coords };
		}
	}

	knight->moves.length = last_move_index;
}

void calculate_bishop_moves(Piece board[ROWS][COLS], Piece* bishop)
{
	Coord moves[] =
	{
		{ 1, 1 },
		{ -1, 1 },
		{ 1, -1 },
		{ -1, -1 }
	};

	int last_move_index = 0;

	for (int i = 0; i < sizeof(moves) / sizeof(Coord); i++)
	{
		Coord new_coords = bishop->coords;

		while (true)
		{
			new_coords.x += moves[i].x;
			new_coords.y += moves[i].y;

			if (!is_valid_coord(new_coords))
			{
				break;
			}

			if (!is_empty(board, new_coords))
			{
				if (board[new_coords.y][new_coords.x].color != bishop->color)
				{
					bishop->moves.moves[last_move_index++] = (Move){ bishop->coords, new_coords };
				}
				break;
			}
			
			bishop->moves.moves[last_move_index++] = (Move){ bishop->coords, new_coords };
		}
	}

	bishop->moves.length = last_move_index;
}

void calculate_rook_moves(Piece board[ROWS][COLS], Piece* rook)
{
	Coord moves[] =
	{
		{ 1, 0 },
		{ -1, 0 },
		{ 0, 1 },
		{ 0, -1 }
	};

	int last_move_index = 0;

	for (int i = 0; i < sizeof(moves) / sizeof(Coord); i++)
	{
		Coord new_coords = rook->coords;

		while (true)
		{
			new_coords.x += moves[i].x;
			new_coords.y += moves[i].y;

			if (!is_valid_coord(new_coords))
			{
				break;
			}

			if (!is_empty(board, new_coords))
			{
				if (board[new_coords.y][new_coords.x].color != rook->color)
				{
					rook->moves.moves[last_move_index++] = (Move){ rook->coords, new_coords };
				}
				break;
			}

			rook->moves.moves[last_move_index++] = (Move){ rook->coords, new_coords };
		}
	}

	rook->moves.length = last_move_index;
}

void calculate_queen_moves(Piece board[ROWS][COLS], Piece* queen)
{
	int last_move_index = 0;

	Coord moves[] =
	{
		{ 1, 1 },
		{ 1, -1 },
		{ -1, -1 },
		{ -1, 1 },
		{ 1, 0 },
		{ -1, 0 },
		{ 0, 1 },
		{ 0, -1 }
	};

	for (int i = 0; i < sizeof(moves) / sizeof(Coord); i++)
	{
		Coord new_coords = queen->coords;

		while (true)
		{
			new_coords.x += moves[i].x;
			new_coords.y += moves[i].y;

			if (!is_valid_coord(new_coords))
			{
				break;
			}

			if (!is_empty(board, new_coords))
			{
				if (board[new_coords.y][new_coords.x].color != queen->color)
				{
					queen->moves.moves[last_move_index++] = (Move){ queen->coords, new_coords };
				}
				break;
			}

			queen->moves.moves[last_move_index++] = (Move){ queen->coords, new_coords };
		}
	}

	queen->moves.length = last_move_index;
}

void calculate_king_moves(Piece board[ROWS][COLS], Piece* king)
{
	int last_move_index = 0;

	Coord moves[] =
	{
		{ 1, 1 },
		{ 1, -1 },
		{ -1, -1 },
		{ -1, 1 },
		{ 1, 0 },
		{ -1, 0 },
		{ 0, 1 },
		{ 0, -1 }
	};

	for (int i = 0; i < sizeof(moves) / sizeof(Coord); i++)
	{
		Coord move = moves[i];

		Coord new_coords = add_coords(king->coords, move);

		if (is_valid_coord(new_coords) && (is_empty(board, new_coords) || board[new_coords.y][new_coords.x].color != king->color))
		{
			king->moves.moves[last_move_index++] = (Move){ king->coords, new_coords };
		}
	}

	king->moves.length = last_move_index;
}