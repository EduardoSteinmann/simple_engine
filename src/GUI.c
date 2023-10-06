#include "GUI.h"

SDL_Surface* get_piece_image(const int piece_value, const int color)
{
    char file_name[30] = "assets/";

    if (color == WHITE)
    {
        strcat_s(file_name, 30, "white");
    }
    else
    {
        strcat_s(file_name, 30, "black");
    }

    if (piece_value == PAWN)
    {
        strcat_s(file_name, 30, "_pawn80x80.png");
    }
    else if (piece_value == KNIGHT)
    {
        strcat_s(file_name, 30, "_knight80x80.png");
    }
    else if (piece_value == BISHOP)
    {
        strcat_s(file_name, 30, "_bishop80x80.png");
    }
    else if (piece_value == ROOK)
    {
        strcat_s(file_name, 30, "_rook80x80.png");
    }
    else if (piece_value == QUEEN)
    {
        strcat_s(file_name, 30, "_queen80x80.png");
    }
    else if (piece_value == KING)
    {
        strcat_s(file_name, 30, "_king80x80.png");
    }

    return IMG_Load(file_name);
}

const int display_start_screen(SDL_Renderer* const renderer)
{
    SDL_Surface* const start_screen = IMG_Load("assets/Chess_Start_Menu.png");

    if (!start_screen)
    {
        return -1;
    }

    const SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    SDL_Texture* const start_texture = SDL_CreateTextureFromSurface(renderer, start_screen);

    if (!start_texture)
    {
        return -1;
    }

    SDL_RenderCopy(renderer, start_texture, NULL, &screen);

    SDL_FreeSurface(start_screen);
    SDL_DestroyTexture(start_texture);

    return 0;
}

const int display_color_selection(SDL_Renderer* const renderer)
{
    SDL_Surface* const color_selection = IMG_Load("assets/color_selection.png");

    if (!color_selection)
    {
        return -1;
    }

    const SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    SDL_Texture* const color_selection_texture = SDL_CreateTextureFromSurface(renderer, color_selection);

    if (!color_selection_texture)
    {
        return -1;
    }

    SDL_RenderCopy(renderer, color_selection_texture, NULL, &screen);

    SDL_FreeSurface(color_selection);
    SDL_DestroyTexture(color_selection_texture);

    return 0;
}

const int display_board(SDL_Renderer* renderer)
{
    SDL_Surface* const board_img = IMG_Load("assets/board.png");

    if (!board_img)
    {
        return -1;
    }

    const SDL_Rect screen = { 0, 0, BOARD_WIDTH, SCREEN_HEIGHT };

    SDL_Texture* const board_texture = SDL_CreateTextureFromSurface(renderer, board_img);

    if (!board_texture)
    {
        return -1;
    }

    SDL_RenderCopy(renderer, board_texture, NULL, &screen);

    SDL_FreeSurface(board_img);
    SDL_DestroyTexture(board_texture);

    return 0;
}

const int display_pieces(SDL_Renderer* const renderer, Piece board[ROWS][COLS], const Piece* dragging_piece)
{
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (!is_empty(board, (Coord) { col, row }))
            {
                //creates texture from the piece's image
                SDL_Surface* piece_image = get_piece_image(get_piece_value(&board[row][col]), board[row][col].color);

                if (!piece_image)
                {
                    return -1;
                }

                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, piece_image);

                if (!texture)
                {
                    return -1;
                }

                //how big the texture can be in terms of an SDL rectangle
                SDL_Rect src = { 0, 0, 80, 80 };

                //Tile on which the image is loaded and fitted on
                SDL_Rect tile = { (col * TILE_SIZE + (TILE_SIZE / 8)), (row * (TILE_SIZE) + (TILE_SIZE / 8)), TILE_SIZE * 3 / 4, TILE_SIZE * 3 / 4 };

                if (dragging_piece != &board[row][col])
                {
                    SDL_RenderCopy(renderer, texture, &src, &tile);
                }

                SDL_FreeSurface(piece_image);
                SDL_DestroyTexture(texture);
            }
        }
    }
    return 0;
}

const int display_all_legal_moves(SDL_Renderer* const renderer, Piece board[ROWS][COLS], const Piece* const piece, const int current_player)
{
    if (piece->color != current_player)
    {
        return 0;
    }

    for (int move_index = 0; move_index < piece->moves.length; move_index++)
    {
        const Move move = piece->moves.moves[move_index];

        const SDL_Rect srcDot = { 0, 0, 80, 80 };

        const SDL_Rect srcCirc = { 0, 0, 120, 120 };

        SDL_Rect tile = { -1, -1, -1, -1 };
        SDL_Surface* image = NULL;

        if (!is_empty(board, move.final))
        {
            tile = (SDL_Rect) { move.final.x * TILE_SIZE, move.final.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            image = IMG_Load("assets/white_circle.png");

            if (!image)
            {
                return -1;
            }
        }
        else
        {
            tile = (SDL_Rect) { (move.final.x * TILE_SIZE) + TILE_SIZE / 8, (move.final.y * TILE_SIZE) + TILE_SIZE / 8, TILE_SIZE * 3 / 4, TILE_SIZE * 3 / 4 };

            image = IMG_Load("assets/gray_dot.png");

            if (!image)
            {
                return -1;
            }
        }

        SDL_Texture* const texture = SDL_CreateTextureFromSurface(renderer, image);

        if (!texture)
        {
            return -1;
        }

        SDL_RenderCopy(renderer, texture, NULL, &tile);

        SDL_DestroyTexture(texture);

        SDL_FreeSurface(image);
    }

    return 0;
}

const int display_dragged_piece(SDL_Renderer* renderer, const Coord mouse, const int piece_value, const int color)
{
    SDL_Surface* piece_image = get_piece_image(piece_value, color);

    if (!piece_image)
    {
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, piece_image);

    if (!texture)
    {
        return -1;
    }

    //how big the texture can be in terms of an SDL rectangle
    SDL_Rect src = { 0, 0, 80, 80 };

    //Tile on which the image is loaded and fitted on
    SDL_Rect tile =
    {
        mouse.x - (TILE_SIZE / 8),
        mouse.y - (TILE_SIZE / 8),
        (int)(TILE_SIZE * 0.75),
        (int)(TILE_SIZE * 0.75)
    };

    SDL_RenderCopy(renderer, texture, &src, &tile);

    SDL_FreeSurface(piece_image);

    SDL_DestroyTexture(texture);

    return 0;
}

const int display_promotion_button(SDL_Renderer* const renderer, const int x_coord, const int y_coord, const int piece_value, const int color)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_Rect tile = { x_coord + (TILE_SIZE / 8), y_coord + (TILE_SIZE / 8), TILE_SIZE * 3 / 4, TILE_SIZE * 3 / 4 };

    SDL_Rect white_background = { x_coord, y_coord, TILE_SIZE, TILE_SIZE };

    SDL_RenderFillRect(renderer, &white_background);

    SDL_Surface* piece_img = get_piece_image(piece_value, color);

    if (!piece_img)
    {
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, piece_img);

    if (!texture)
    {
        return -1;
    }

    SDL_RenderCopy(renderer, texture, NULL, &tile);

    SDL_FreeSurface(piece_img);
    SDL_DestroyTexture(texture);

    return 0;
}

const int display_checkmate_screen(SDL_Renderer* const renderer, const int winner)
{
    const char* file_name = winner == WHITE ? "assets/white_checkmate.png" : "assets/black_checkmate.png";

    SDL_Surface* checkmate_img = IMG_Load(file_name);

    if (!checkmate_img)
    {
        return -1;
    }

    const SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    SDL_Texture* const checkmate_texture = SDL_CreateTextureFromSurface(renderer, checkmate_img);

    if (!checkmate_texture)
    {
        return -1;
    }

    SDL_RenderCopy(renderer, checkmate_texture, NULL, &screen);

    SDL_FreeSurface(checkmate_img);
    SDL_DestroyTexture(checkmate_texture);

    return 0;
}

const int display_stalemate_screen(SDL_Renderer* const renderer)
{
    const char* file_name = "assets/stalemate.png";

    SDL_Surface* stalemate_img = IMG_Load(file_name);

    if (!stalemate_img)
    {
        return -1;
    }

    const SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    SDL_Texture* const stalemate_texture = SDL_CreateTextureFromSurface(renderer, stalemate_img);

    if (!stalemate_texture)
    {
        return -1;
    }

    SDL_RenderCopy(renderer, stalemate_texture, NULL, &screen);

    SDL_FreeSurface(stalemate_img);
    SDL_DestroyTexture(stalemate_texture);

    return 0;
}