#pragma once

#include <SFML/Graphics.hpp>
#include "game.h"
#include <map>

class Gui {
public:
    Gui(Game& game);
    void run();
    // Add more as needed

private:
    Game& game;
    sf::RenderWindow window;

    // Add textures, sprites, etc. as needed
    std::map<std::string, sf::Texture> squareTextures;

    std::map<std::string, sf::Texture> letterTextures;

    std::map<std::string, sf::Texture> digitTextures;

    std::map<std::string, sf::Texture> pieceTextures;

    void drawBoard();
    void drawPieces();
    // Add event handling methods as needed
};
