#pragma once

#include <SFML/Graphics.hpp>
#include "game.h"

class Gui {
public:
    Gui(Game& game);
    void run();
    // Add more as needed

private:
    Game& game;
    sf::RenderWindow window;
    // Add textures, sprites, etc. as needed

    void drawBoard();
    void drawPieces();
    // Add event handling methods as needed
};
