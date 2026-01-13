#pragma once

#include <SFML/Graphics.hpp>
#include "game.h"
#include <map>

class Gui {
public:
    Gui(Game& game);
    void run();

private:
    Game& game;
    sf::RenderWindow window;

    std::map<std::string, sf::Texture> squareTextures;
    std::map<std::string, sf::Texture> letterTextures;
    std::map<std::string, sf::Texture> digitTextures;
    std::map<std::string, sf::Texture> pieceTextures;
    std::map<std::string, sf::Texture> utilTextures;

	sf::Vector2f selectedSquare;

    void drawBoard();
    void drawPieces();
};
