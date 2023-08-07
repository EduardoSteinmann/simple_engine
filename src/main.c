#include "game.h"

int main(int argc, char** argv)
{
	while (true)
	{
		const int game_result = game_loop();

		if (game_result == QUIT_MODE || game_result == -1)
		{
			break;
		}
	}
	
	SDL_Quit();

	return 0;
}