#pragma once

#include <SFML/Graphics.hpp>
#include "game.h"
#include <map>
#include <vector>
#include <string>

/**
 * @file gui.h
 * @brief GUI layer using SFML to render board and handle user input.
 */

/** Manages the application window, rendering and input for a Game. */
class Gui {
public:
	/** Construct GUI bound to a Game instance. */
	Gui(Game& game);

	/** Run the main loop. */
	void run();

private:
	Game& game;
	sf::RenderWindow window;

	// Texture containers for board rendering
	std::map<std::string, sf::Texture> squareTextures;
	std::map<std::string, sf::Texture> letterTextures;
	std::map<std::string, sf::Texture> digitTextures;
	std::map<std::string, sf::Texture> pieceTextures;
	std::map<std::string, sf::Texture> utilTextures;

	sf::Vector2i selectedSquare;
	sf::Vector2i selectedPiece;

	// Promotion state
	bool isPromotionPending;
	sf::Vector2i promotionSquare;

	// Analysis state after a finished game
	bool isAnalyzing;

	// Manual board orientation state
	bool boardFlipped;

	sf::Font font;

	void drawBoard();
	void drawPieces();
	void drawMenu();
};
