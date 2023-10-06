#include "game.h"

const int start_screen(const SDL_Event* const event, const Coord* const mouse)
{
    if (event->type == SDL_QUIT)
    {
        return QUIT_MODE;
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (is_PVP_clicked(mouse))
        {
            return PVP_MODE;
        }
        else if (is_AI_clicked(mouse))
        {
            return COLOR_SELECTION;
        }
    }

    return START_SCREEN;
}

const int color_selection(SDL_Renderer* const renderer, const SDL_Event* const event, const Coord* const mouse, int* const player_color, int* const AI_color)
{
    if (display_color_selection(renderer))
    {
        return -1;
    }

    SDL_RenderPresent(renderer);

    if (event->type == SDL_QUIT)
    {
        return QUIT_MODE;
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (is_white_clicked(mouse))
        {
            *player_color = WHITE;
            *AI_color = BLACK;
            return AI_MODE;
        }
        else if (is_black_clicked(mouse))
        {
            *player_color = BLACK;
            *AI_color = WHITE;
            return AI_MODE;
        }
    }

    return COLOR_SELECTION;
}

const bool is_PVP_clicked(const Coord* const mouse)
{
    return mouse->x >= 400 && mouse->x <= 600 && mouse->y >= 350 && mouse->y <= 450;
}

const bool is_AI_clicked(const Coord* const mouse)
{
    return mouse->x >= 400 && mouse->x <= 600 && mouse->y >= 575 && mouse->y <= 650;
}

const bool is_white_clicked(const Coord* const mouse)
{
    return mouse->x >= 420 && mouse->x <= 580 && mouse->y >= 320 && mouse->y <= 480;
}

const bool is_black_clicked(const Coord* const mouse)
{
    return mouse->x >= 420 && mouse->x <= 580 && mouse->y >= 520 && mouse->y <= 680;
}

const int PVP_mode(Piece board[ROWS][COLS], const SDL_Event* const event, Coord* const mouse, SDL_Renderer* const renderer, int* const turn)
{
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    const SDL_Rect tile = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    SDL_RenderFillRect(renderer, &tile);

    static bool is_promoting = false;

    static Piece pawn = { -1, -1, -1, -1 };

    static Piece* clicked_piece = NULL;

    if (is_promoting)
    {
        if (pawn_promotion(board, &pawn, event, mouse, renderer, &is_promoting))
        {
            return -1;
        }

        return PVP_MODE;
    }

    int current_player = EMPTY;

    if (*turn % 2 == 0)
    {
        current_player = WHITE;
    }
    else
    {
        current_player = BLACK;
    }
    
    Piece* const current_king = get_king(board, current_player);

    const bool in_check = is_king_in_check(current_king, board);

    calculate_all_moves_for_a_side(board, current_king, in_check, current_player);

    if (in_check_mate(board, in_check, current_player))
    {
        return CHECKMATE;
    }
    else if (in_stale_mate(board, in_check, current_player))
    {
        return STALEMATE;
    }

    if (event->type == SDL_QUIT)
    {
        return QUIT_MODE;
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&mouse->x, &mouse->y);

        Coord clicked = { mouse->x / TILE_SIZE, mouse->y / TILE_SIZE };

        if (is_valid_row_and_col(clicked.x, clicked.y) && !is_empty(board, clicked))
        {
            clicked_piece = &board[clicked.y][clicked.x];
        }
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        if (!clicked_piece)
        {
            return PVP_MODE;
        }

        SDL_GetMouseState(&mouse->x, &mouse->y);

        if (display_board(renderer))
        {
            return -1;
        }

        if (display_pieces(renderer, board, clicked_piece))
        {
            return -1;
        }

        if (display_all_legal_moves(renderer, board, clicked_piece, current_player))
        {
            return -1;
        }

        if (display_dragged_piece(renderer, *mouse, get_piece_value(clicked_piece), clicked_piece->color))
        {
            return -1;
        }

        SDL_RenderPresent(renderer);
    }
    else if (event->type == SDL_MOUSEBUTTONUP)
    {
        if (clicked_piece)
        {
            SDL_GetMouseState(&mouse->x, &mouse->y);

            const Move move = { clicked_piece->coords, { mouse->x / TILE_SIZE, mouse->y / TILE_SIZE } };

            const int move_code = make_move(board, move, current_player, false);

            clicked_piece = NULL;

            if (move_code)
            {
                (*turn)++;

                if (move_code == PAWN_PROMOTION)
                {
                    is_promoting = true;
                    pawn = init_piece(PAWN, current_player, move.final.x, move.final.y);
                    return PVP_MODE;
                }
            }
        }

        if (display_board(renderer))
        {
            return -1;
        }

        if (display_pieces(renderer, board, clicked_piece))
        {
            return -1;
        }

        SDL_RenderPresent(renderer);
    }

    return PVP_MODE;
}

const int player_move(Piece board[ROWS][COLS], const SDL_Event* const event, Coord* const mouse, SDL_Renderer* const renderer, int* const turn, const int player_color)
{
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    const SDL_Rect tile = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    SDL_RenderFillRect(renderer, &tile);

    int current_player = EMPTY;

    static bool is_promoting = false;

    static Piece pawn = { -1, -1, -1, -1 };

    static Piece* clicked_piece = NULL;

    if (is_promoting)
    {
        if (pawn_promotion(board, &pawn, event, mouse, renderer, &is_promoting))
        {
            return -1;
        }

        return AI_MODE;
    }

    if (*turn % 2 == 0)
    {
        current_player = WHITE;
    }
    else
    {
        current_player = BLACK;
    }

    if (current_player != player_color)
    {
        return AI_MODE;
    }

    Piece* const current_king = get_king(board, current_player);

    const bool in_check = is_king_in_check(current_king, board);

    calculate_all_moves_for_a_side(board, current_king, in_check, current_player);

    if (in_check_mate(board, in_check, current_player))
    {
        SDL_Delay(1000);
        return CHECKMATE;
    }
    else if (in_stale_mate(board, in_check, current_player))
    {
        SDL_Delay(1000);
        return STALEMATE;
    }

    if (event->type == SDL_QUIT)
    {
        return QUIT_MODE;
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&mouse->x, &mouse->y);

        Coord clicked = { mouse->x / TILE_SIZE, mouse->y / TILE_SIZE };

        if (is_valid_row_and_col(clicked.x, clicked.y) && !is_empty(board, clicked))
        {
            clicked_piece = &board[clicked.y][clicked.x];
        }
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        if (!clicked_piece)
        {
            return AI_MODE;
        }

        SDL_GetMouseState(&mouse->x, &mouse->y);

        if (display_board(renderer))
        {
            return -1;
        }

        if (display_pieces(renderer, board, clicked_piece))
        {
            return -1;
        }

        if (display_all_legal_moves(renderer, board, clicked_piece, current_player))
        {
            return -1;
        }

        if (display_dragged_piece(renderer, *mouse, get_piece_value(clicked_piece), clicked_piece->color))
        {
            return -1;
        }

        SDL_RenderPresent(renderer);
    }
    else if (event->type == SDL_MOUSEBUTTONUP)
    {
        if (clicked_piece)
        {
            SDL_GetMouseState(&mouse->x, &mouse->y);

            const Move move = { clicked_piece->coords, { mouse->x / TILE_SIZE, mouse->y / TILE_SIZE } };

            const int move_code = make_move(board, move, current_player, false);

            clicked_piece = NULL;

            if (move_code)
            {
                (*turn)++;

                if (move_code == PAWN_PROMOTION)
                {
                    is_promoting = true;
                    pawn = init_piece(PAWN, current_player, move.final.x, move.final.y);
                    return PAWN_PROMOTION;
                }
            }
        }

        if (display_board(renderer))
        {
            return -1;
        }

        if (display_pieces(renderer, board, clicked_piece))
        {
            return -1;
        }

        SDL_RenderPresent(renderer);
    }

    return AI_MODE;
}

const int AI_move(Piece board[ROWS][COLS], SDL_Renderer* const renderer, const int AI_color, int* const turn)
{
    int current_player = -1;

    if (*turn % 2 == 0)
    {
        current_player = WHITE;
    }
    else
    {
        current_player = BLACK;
    }

    if (current_player != AI_color)
    {
        return AI_MODE;
    }

    Piece* const current_king = get_king(board, AI_color);

    const bool in_check = is_king_in_check(current_king, board);

    calculate_all_moves_for_a_side(board, current_king, in_check, AI_color);

    if (in_check_mate(board, in_check, AI_color))
    {
        SDL_Delay(1000);
        return CHECKMATE;
    }
    else if (in_stale_mate(board, in_check, AI_color))
    {
        SDL_Delay(1000);
        return STALEMATE;
    }

    const Move AI_move = generate_move(board, AI_color, SEARCH_DEPTH);

    const int move_code = make_move(board, AI_move, AI_color, false);

    if (!move_code)
    {
        return -1;
    }

    (*turn)++;

    if (move_code == PAWN_PROMOTION)
    {
        board[AI_move.final.y][AI_move.final.x] = init_piece(QUEEN, board[AI_move.final.y][AI_move.final.x].color, board[AI_move.final.y][AI_move.final.x].coords.x, board[AI_move.final.y][AI_move.final.x].coords.y);
    }

    if (display_board(renderer))
    {
        return -1;
    }

    if (display_pieces(renderer, board, NULL))
    {
        return -1;
    }

    SDL_RenderPresent(renderer);

    return AI_MODE;
}

const int pawn_promotion(Piece board[ROWS][COLS], const Piece* const pawn, const SDL_Event* const event, Coord* const mouse, SDL_Renderer* const renderer, bool* const is_promoting)
{
    const int pawn_y = pawn->coords.y;
    const int pawn_x = pawn->coords.x;

    const int button_x = pawn_x * TILE_SIZE;
    
    if (event->type == SDL_QUIT)
    {
        return QUIT_MODE;
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        if (display_board(renderer))
        {
            return -1;
        }

        if (display_pieces(renderer, board, NULL))
        {
            return -1;
        }

        if (display_promotion_button(renderer, button_x, get_promotion_y_coord(pawn->coords.y, pawn->color, 1), QUEEN, pawn->color))
        {
            return -1;
        }

        if (display_promotion_button(renderer, button_x, get_promotion_y_coord(pawn->coords.y, pawn->color, 2), ROOK, pawn->color))
        {
            return -1;
        }

        if (display_promotion_button(renderer, button_x, get_promotion_y_coord(pawn->coords.y, pawn->color, 3), BISHOP, pawn->color))
        {
            return -1;
        }

        if (display_promotion_button(renderer, button_x, get_promotion_y_coord(pawn->coords.y, pawn->color, 4), KNIGHT, pawn->color))
        {
            return -1;
        }

        SDL_RenderPresent(renderer);
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (is_promotion_button_clicked(mouse, button_x, get_promotion_y_coord(pawn->coords.y, pawn->color, 1)))
        {
            board[pawn->coords.y][pawn->coords.x] = init_piece(QUEEN, pawn->color, pawn->coords.x, pawn->coords.y);
            *is_promoting = false;
        }
        else if (is_promotion_button_clicked(mouse, button_x, get_promotion_y_coord(pawn->coords.y, pawn->color, 2)))
        {
            board[pawn->coords.y][pawn->coords.x] = init_piece(ROOK, pawn->color, pawn->coords.x, pawn->coords.y);
            *is_promoting = false;
        }
        else if (is_promotion_button_clicked(mouse, button_x, get_promotion_y_coord(pawn->coords.y, pawn->color, 3)))
        {
            board[pawn->coords.y][pawn->coords.x] = init_piece(BISHOP, pawn->color, pawn->coords.x, pawn->coords.y);
            *is_promoting = false;
        }
        else if (is_promotion_button_clicked(mouse, button_x, get_promotion_y_coord(pawn->coords.y, pawn->color, 4)))
        {
            board[pawn->coords.y][pawn->coords.x] = init_piece(KNIGHT, pawn->color, pawn->coords.x, pawn->coords.y);
            *is_promoting = false;
        }
    }

    return 0;
}

const int get_promotion_y_coord(const int pawn_y, const int color, const int num_on_menu)
{
    return (pawn_y - (color * num_on_menu)) * TILE_SIZE;
}

const bool is_promotion_button_clicked(const Coord* const mouse, const int x_coord, const int y_coord)
{
    return x_coord / TILE_SIZE == mouse->x / TILE_SIZE && y_coord / TILE_SIZE == mouse->y / TILE_SIZE;
}

const int checkmate_screen(SDL_Renderer* const renderer, const SDL_Event* const event, const Coord* const mouse, const int* const turn)
{
    int winner = 0;

    if ((*turn - 1) % 2 == 0)
    {
        winner = WHITE;
    }
    else
    {
        winner = BLACK;
    }

    if (display_checkmate_screen(renderer, winner))
    {
        return -1;
    }

    SDL_RenderPresent(renderer);

    if (event->type == SDL_QUIT)
    {
        return QUIT_MODE;
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (is_back_to_start_clicked(mouse))
        {
            return BACK_TO_START;
        }
    }

    return CHECKMATE;
}

const int stalemate_screen(SDL_Renderer* const renderer, const SDL_Event* const event, const Coord* const mouse)
{
    if (display_stalemate_screen(renderer))
    {
        return -1;
    }

    SDL_RenderPresent(renderer);

    if (event->type == SDL_QUIT)
    {
        return QUIT_MODE;
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (is_back_to_start_clicked(mouse))
        {
            return BACK_TO_START;
        }
    }

    return STALEMATE;
}

const bool is_back_to_start_clicked(const Coord* const mouse)
{
    return mouse->x >= 420 && mouse->x <= 580 && mouse->y >= 320 && mouse->y <= 480;
}

const int game_loop()
{
    Piece board[ROWS][COLS];

    init_new_board(board);

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        return -1;
    }

    SDL_Window* const window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if (!window)
    {
        return -1;
    }

    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (!renderer)
    {
        return -1;
    }

    SDL_Surface* const icon = IMG_Load("assets/icon.png");

    if (!icon)
    {
        return -1;
    }

    SDL_SetWindowIcon(window, icon);

    SDL_Event event;

    if (display_start_screen(renderer))
    {
        return -1;
    }

    SDL_RenderPresent(renderer);

    int mode = START_SCREEN;

    int turn = 0;

    int AI_color = BLACK;
    int player_color = WHITE;

    int result = 0;

    while (SDL_WaitEvent(&event))
    {
        Coord mouse = { -1, -1 };

        SDL_GetMouseState(&mouse.x, &mouse.y);

        if (mode == QUIT_MODE)
        {
            result = QUIT_MODE;
            break;
        }
        else if (mode == -1)
        {
            result = -1;
            break;
        }
        else if (mode == START_SCREEN)
        {
            mode = start_screen(&event, &mouse);
        }
        else if (mode == COLOR_SELECTION)
        {
            mode = color_selection(renderer, &event, &mouse, &player_color, &AI_color);
        }
        else if (mode == PVP_MODE)
        {
            mode = PVP_mode(board, &event, &mouse, renderer, &turn);
        }
        else if (mode == AI_MODE || mode == PAWN_PROMOTION)
        {
            const int player_result = player_move(board, &event, &mouse, renderer, &turn, player_color);

            if (player_result == PAWN_PROMOTION || player_result == CHECKMATE || player_result == STALEMATE || player_result == QUIT_MODE || player_result == -1)
            {
                mode = player_result;
                continue;
            }

            mode = AI_move(board, renderer, AI_color, &turn);
        }
        else if (mode == CHECKMATE)
        {
            mode = checkmate_screen(renderer, &event, &mouse, &turn);
        }
        else if (mode == STALEMATE)
        {
            mode = stalemate_screen(renderer, &event, &mouse);
        }
        else if (mode == BACK_TO_START)
        {
            break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return result;
}