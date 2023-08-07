#include "AI_calculation.h"

const int eval_board(Piece board[ROWS][COLS])
{
    int eval = 0;

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (!is_empty(board, (Coord) { col, row }))
            {
                int value = get_piece_value_on_board(get_piece_value(&board[row][col]), board[row][col].color, board[row][col].coords);
                eval += value * -1 * board[row][col].color;
            }
        }
    }
    return eval;
}

const int test_move(Piece board[ROWS][COLS], const Move move)
{
    int move_code = 0;

    Piece* piece = &board[move.initial.y][move.initial.x];

    const bool is_pawn = get_piece_value(piece) == PAWN;

    move_code = REGULAR_MOVE;

    if (is_pawn)
    {
        if (move.final.y == move.initial.y + (2 * piece->color))
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

    if (is_occupied_by_enemy_piece(board, piece->color, move.final))
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
        board[move.final.y][move.final.x] = init_piece(QUEEN, piece->color, move.final.x, move.final.y);
    }

    remove_en_passant_if_not_done_immediately(board, piece->color);

    return move_code;
}

const int max(const int l_value, const int r_value)
{
    return l_value > r_value ? l_value : r_value;
}

const int min(const int l_value, const int r_value)
{
    return l_value < r_value ? l_value : r_value;
}

//Precondition: All moves for board for side color have been calculated
const Move generate_move(Piece board[ROWS][COLS], const int color, const int search_depth)
{
    Move result = EMPTY_MOVE;

    int max_eval = -INFINITY;

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (!is_occupied_by_allied_piece(board, color, (Coord) { col, row }))
            {
                continue;
            }

            const Piece piece = board[row][col];

            for (int move_index = 0; move_index < piece.moves.length; move_index++)
            {
                const Move move = piece.moves.moves[move_index];
                Piece copy_board[ROWS][COLS];
                clone_board(board, copy_board);

                test_move(copy_board, move);

                const int eval = minimax(copy_board, get_opposite_color(color), search_depth - 1, -INFINITY, INFINITY) * color * -1;

                if (eval >= max_eval)
                {
                    max_eval = eval;
                    result = move;
                }
            }
        }
    }

    return result;
}

const int minimax(Piece board[ROWS][COLS], const int color, const int search_depth, int max_assured_score, int min_assured_score)
{
    if (search_depth == 0)
    {
        return eval_board(board);
    }

    int max_min_eval = color == WHITE ? -INFINITY : INFINITY;

    Piece* king = get_king(board, color);

    if (!king)
    {
        return INFINITY * color * -1; //Fail safe: if get_king ever returns null, return the least favorable eval for the caller of minimax
    }

    const bool in_check = is_king_in_check(king, board);

    calculate_all_moves_for_a_side(board, king, in_check, color);

    if(in_check_mate(board, in_check, color))
    {
        return (INFINITY + 1) * color;
    }

    if (in_stale_mate(board, in_check, color))
    {
        return eval_board(board);
    }

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (!is_occupied_by_allied_piece(board, color, (Coord) { col, row }))
            {
                continue;
            }

            const Piece piece = board[row][col];

            for (int move_index = 0; move_index < piece.moves.length; move_index++)
            {
                const Move move = piece.moves.moves[move_index];

                Piece copy_board[ROWS][COLS];
                clone_board(board, copy_board);

                test_move(copy_board, move);

                const int eval = minimax(copy_board, get_opposite_color(color), search_depth - 1, max_assured_score, min_assured_score);

                if (color == WHITE)
                {
                    max_min_eval = max(max_min_eval, eval);
                    max_assured_score = max(eval, max_assured_score);
                }
                else
                {
                    max_min_eval = min(max_min_eval, eval);
                    min_assured_score = min(eval, min_assured_score);
                }

                if (min_assured_score <= max_assured_score)
                {
                    return max_min_eval;
                }
            }
        }
    }

    return max_min_eval;
}