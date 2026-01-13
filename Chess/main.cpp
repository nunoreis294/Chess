#include "game.h"
#include "gui.h"

int main()
{
	// Initialize game
    Game game;
	game.newGame();

	// Initialize GUI
    Gui gui(game);
    gui.run();
}