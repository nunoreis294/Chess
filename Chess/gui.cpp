#include <SFML/Graphics.hpp>
#include <iostream>
#include "gui.h"

// Constructor: initialize the window and any resources
Gui::Gui(Game& game)
    : game(game), window(sf::VideoMode({ 600, 600 }), "Chess")
{
    // Load textures, set up sprites, etc. (if needed)
}

// Main loop: handle events and draw the board
void Gui::run()
{
	const Board& board = game.getBoard();

    // Print the board to the console
    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; x++) {
		    Piece piece = board.getPiece(x, y);
            
            char c = ' ';

            if (piece.type != PieceType::None) {
                c = piece.type == PieceType::Pawn ? 'P' :
                    piece.type == PieceType::Rook ? 'R' :
                    piece.type == PieceType::Knight ? 'N' :
                    piece.type == PieceType::Bishop ? 'B' :
                    piece.type == PieceType::Queen ? 'Q' : 'K';
                
                if (piece.color == Color::Black) {
                    c = std::tolower(c);
                }
			}

			std::cout << c << ' ';
        }

		std::cout << std::endl;
    }
}

// Draw the chessboard squares
void Gui::drawBoard()
{

}

// Draw the chess pieces (placeholder: you would use textures/sprites in a real app)
void Gui::drawPieces()
{

}

/*sf::RenderWindow window(sf::VideoMode({200, 200}), "SFML works!");
sf::CircleShape shape(100.f);
shape.setFillColor(sf::Color::Green);

while (window.isOpen())
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }

    window.clear();
    window.draw(shape);
    window.display();
}*/