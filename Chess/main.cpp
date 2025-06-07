#include "game.h"
#include "gui.h"

int main()
{
    Game game;
	game.newGame();

    Gui gui(game);
    gui.run();
}