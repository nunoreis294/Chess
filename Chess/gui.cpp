#include <iostream>
#include "gui.h"

// Constructor
Gui::Gui(Game& game)
    : game(game), window(sf::VideoMode({ 900, 600 }), "Chess")
{
	selectedSquare = sf::Vector2f(-1.f, -1.f);
	selectedPiece = sf::Vector2f(-1.f, -1.f);

	// Load board square textures
    const std::string squareColors[] = { "white", "black", "brown" };
    const std::string squareTypes[] = { "", "-side", "-corner"};

    for (const auto& color : squareColors)
    {
        for (const auto& type : squareTypes)
        {
			// Brown squares only have side and corner types (black and white have all three)
            if (!(type == "" && color == "brown"))
            {
                std::string key = color + type;
                std::string filename = "images/square-" + key + ".png";
                sf::Texture texture;

                if (!texture.loadFromFile(filename))
                {
                    std::cerr << "Failed to load " << filename << std::endl;
                }

                squareTextures[key] = texture;
			}
        }
    }

	// Load letter textures
    const std::string letters[] = { "a", "b", "c", "d", "e", "f", "g", "h" };

    for (const auto& letter : letters)
    {
        std::string key = letter;
        std::string filename = "images/" + key + "-letter.png";
        sf::Texture texture;

        if (!texture.loadFromFile(filename))
        {
            std::cerr << "Failed to load " << filename << std::endl;
        }

        letterTextures[key] = texture;
    }

    // Load digit textures
    const std::string digits[] = { "1", "2", "3", "4", "5", "6", "7", "8" };

    for (const auto& digit : digits)
    {
        std::string key = digit;
        std::string filename = "images/" + key + "-digit.png";
        sf::Texture texture;

        if (!texture.loadFromFile(filename))
        {
            std::cerr << "Failed to load " << filename << std::endl;
        }

        digitTextures[key] = texture;
    }

	// Load utility textures (possible squares, attacked squares and checked)
	const std::string utilNames[] = { "dot", "attacked", "check"};

    for (const auto& name : utilNames)
    {
        std::string key = name;
        std::string filename = "images/" + key + ".png";
        sf::Texture texture;
        if (!texture.loadFromFile(filename))
        {
            std::cerr << "Failed to load " << filename << std::endl;
        }
        utilTextures[key] = texture;
	}

	// Load piece textures
    const std::string pieceColors[] = { "white", "black" };
    const std::string pieceTypes[] = { "pawn", "rook", "knight", "bishop", "queen", "king" };

    for (const auto& color : pieceColors)
    {
        for (const auto& type : pieceTypes)
        {
            std::string key = type + "-" + color;
            std::string filename = "images/" + key + ".png";
            sf::Texture texture;

            if (!texture.loadFromFile(filename))
            {
                std::cerr << "Failed to load " << filename << std::endl;
            }

            pieceTextures[key] = texture;
        }
    }
}

// Run the GUI main loop
void Gui::run()
{
	const Board& board = game.getBoard();

	// Main loop
    while (window.isOpen())
    {
		// Process events
        while (const std::optional event = window.pollEvent())
        {
			// Close window : exit
            if (event->is<sf::Event::Closed>())
                window.close();

			if (event->is<sf::Event::MouseButtonPressed>())
            {
				// Get mouse position
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);

				// Calculate selected square based on mouse position
				const float tileSize = window.getSize().y / 10;
				int x = mousePos.x / tileSize;
				int y = mousePos.y / tileSize;

                if (x > 0 && x <= 8 && y > 0 && y <= 8)
                {
                    selectedSquare = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));

                    Piece piece = board.getPiece(x - 1, y - 1);
                    std::string currentPlayerColor = game.getCurrentPlayerColor();

					//Can select piece if there is no piece currently selected or if selecting own piece
                    if ((currentPlayerColor == "White" && piece.color == PieceColor::White) ||
                        (currentPlayerColor == "Black" && piece.color == PieceColor::Black))
                    {
                        selectedPiece = selectedSquare;
                    }
					else if (selectedPiece.x != -1.f && selectedPiece.y != -1.f)
                    {
                        std::vector<sf::Vector2f> possibleSquares = { board.getPossibleSquares(selectedPiece) };

                        std::vector<sf::Vector2f> attackedSquares = { board.getAttackedSquares(selectedPiece) };

						possibleSquares.insert(possibleSquares.end(), attackedSquares.begin(), attackedSquares.end());

                        for (sf::Vector2f possibleSquare : possibleSquares)
                        {
                            if (possibleSquare.x == selectedSquare.x && possibleSquare.y == selectedSquare.y)
                            {
                                std::cout << "Move\n";
                                break;
							}
                        }

                        selectedPiece = sf::Vector2f(-1.f, -1.f);
					}
                }
                else
                    selectedPiece = sf::Vector2f(-1.f, -1.f);
            }
        }

		// Clear the window
        window.clear(sf::Color::White);

		// Draw the board and pieces
        drawBoard();
        drawPieces();

		// Draw additional GUI elements (menu, moves, etc.)
        

		// Update the window
        window.display();
    }
}

// Draw the chess board (squares, letters, digits, possible moves and attacked squares)
void Gui::drawBoard()
{
    const Board& board = game.getBoard();

	// Use absolute size to ensure squares are always square
	// 8 rows + 2 for borders
    const float tileSize = window.getSize().y / 10;

	// Determine if player is white or black to adjust board orientation
    bool playerWhiteColor = ("White" == game.getCurrentPlayerColor()) ? true : false;

    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            sf::Sprite boardSprite{ squareTextures["white"] };

			// Calculate position based on square size
			sf::Vector2f position(x * tileSize, y * tileSize);

            // Draw squares
            if (y == 0 || y == 9 || x == 0 || x == 9)
            {
				// Brown border squares
                if (y == x || (x == 0 && y == 9) || (x == 9 && y == 0))
                {
                    boardSprite.setTexture(squareTextures["brown-corner"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["brown-corner"].getSize().x), (tileSize / squareTextures["brown-corner"].getSize().y)));
                }
                else
                {
                    boardSprite.setTexture(squareTextures["brown-side"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["brown-side"].getSize().x), (tileSize / squareTextures["brown-side"].getSize().y)));

					// Rotate border sides
                    if (y == 0 || y == 9)
                    {
                        boardSprite.setOrigin(sf::Vector2f(0.f, 32.f));
                        boardSprite.setRotation(sf::degrees(90.f));
                    }
                }
            }            
            else if ((x + y) % 2 == 0)
            {
				// White squares
                if ((x == 1 && y == 1) || (x == 8 && y == 8))
                {
                    boardSprite.setTexture(squareTextures["white-corner"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["white-corner"].getSize().x), (tileSize / squareTextures["white-corner"].getSize().y)));

					// Rotate corners
                    if (x == 8 && y == 8)
                    {
                        boardSprite.setOrigin(sf::Vector2f(32.f, 32.f));
                        boardSprite.setRotation(sf::degrees(180.f));
                    }
                }
                else if (y == 1 || y == 8 || x == 1 || x == 8)
                {
                    boardSprite.setTexture(squareTextures["white-side"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["white-side"].getSize().x), (tileSize / squareTextures["white-side"].getSize().y)));

					// Rotate sides
                    if (x == 8)
                    {
                        boardSprite.setOrigin(sf::Vector2f(32.f, 32.f));
                        boardSprite.setRotation(sf::degrees(180.f));
                    }
                    else if(y == 1)
                    {
                        boardSprite.setOrigin(sf::Vector2f(0.f, 32.f));
                        boardSprite.setRotation(sf::degrees(90.f));
                    }
                    else if (y == 8)
                    {
                        boardSprite.setOrigin(sf::Vector2f(32.f, 0.f));
                        boardSprite.setRotation(sf::degrees(270.f));
                    }
                }
                else
                {
                    boardSprite.setTexture(squareTextures["white"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["white"].getSize().x), (tileSize / squareTextures["white"].getSize().y)));
                }
            }
            else
            {
				// Black squares
                if ((x == 1 && y == 8) || (x == 8 && y == 1))
                {
                    boardSprite.setTexture(squareTextures["black-corner"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["black-corner"].getSize().x), (tileSize / squareTextures["black-corner"].getSize().y)));

					// Rotate corners
                    if (x == 1 && y == 8)
                    {
                        boardSprite.setOrigin(sf::Vector2f(32.f, 0.f));
                        boardSprite.setRotation(sf::degrees(270.f));
                    }
                    else
                    {
                        boardSprite.setOrigin(sf::Vector2f(0.f, 32.f));
                        boardSprite.setRotation(sf::degrees(90.f));
                    }
                }
                else if (y == 1 || y == 8 || x == 1 || x == 8)
                {
                    boardSprite.setTexture(squareTextures["black-side"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["black-side"].getSize().x), (tileSize / squareTextures["black-side"].getSize().y)));

					// Rotate sides
                    if (x == 8)
                    {
                        boardSprite.setOrigin(sf::Vector2f(32.f, 32.f));
                        boardSprite.setRotation(sf::degrees(180.f));
                    }
                    else if (y == 1)
                    {
                        boardSprite.setOrigin(sf::Vector2f(0.f, 32.f));
                        boardSprite.setRotation(sf::degrees(90.f));
                    }
                    else if (y == 8)
                    {
                        boardSprite.setOrigin(sf::Vector2f(32.f, 0.f));
                        boardSprite.setRotation(sf::degrees(270.f));
                    }
                }
                else
                {
                    boardSprite.setTexture(squareTextures["black"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["black"].getSize().x), (tileSize / squareTextures["black"].getSize().y)));
                }
            }

			// Draw the square
            window.draw(boardSprite);

			// Draw letters and digits on borders
            const std::string letters[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
            const std::string digits[] = { "1", "2", "3", "4", "5", "6", "7", "8" };

            sf::Sprite letterDigitSprite{ letterTextures["a"] };

            if ((x == 0 && (y != 0 && y != 9)) || (y == 9 && (x != 0 && x != 9)))
            {
                letterDigitSprite.setPosition(position);

                if (y == 9 && (x != 0 && x != 9))
                {
					// Letters at bottom (a-h if player is white, h-a if black)
                    int index = playerWhiteColor ? x - 1 : 8 - x;
                    std::string letter = letters[index];

                    letterDigitSprite.setTexture(letterTextures[letter]);
                    letterDigitSprite.setPosition(position);
                    letterDigitSprite.setScale(sf::Vector2f((tileSize / letterTextures[letter].getSize().x), (tileSize / letterTextures[letter].getSize().y)));
                }
                else if (x == 0 && (y != 0 && y != 9))
                {
					// Digits at left (1-8 if player is white, 8-1 if black)
                    int index = playerWhiteColor ? 8 - y : y - 1;
                    std::string digit = digits[index];

                    letterDigitSprite.setTexture(digitTextures[digit]);
                    letterDigitSprite.setPosition(position);
                    letterDigitSprite.setScale(sf::Vector2f((tileSize / digitTextures[digit].getSize().x), (tileSize / digitTextures[digit].getSize().y)));
                }

                window.draw(letterDigitSprite);
            }
        }
    }

	// Draw possible squares (dot) and attacked squares (red boarder) based on selected piece
    std::vector<sf::Vector2f> possibleSquares = { board.getPossibleSquares(selectedPiece) };

    sf::Sprite utilSprite{ utilTextures["dot"] };

    for (sf::Vector2f possibleSquare : possibleSquares)
    {
        utilSprite.setPosition(sf::Vector2f(tileSize * possibleSquare.x , tileSize* possibleSquare.y));
        utilSprite.setScale(sf::Vector2f((tileSize / utilTextures["dot"].getSize().x), (tileSize / utilTextures["dot"].getSize().y)));
        window.draw(utilSprite);
    }

    std::vector<sf::Vector2f> attackedSquares = { board.getAttackedSquares(selectedPiece) };

	utilSprite = sf::Sprite{ utilTextures["attacked"] };

    for (sf::Vector2f attackedSquare : attackedSquares)
    {
        utilSprite.setPosition(sf::Vector2f(tileSize* attackedSquare.x, tileSize* attackedSquare.y));
        utilSprite.setScale(sf::Vector2f((tileSize / utilTextures["attacked"].getSize().x), (tileSize / utilTextures["attacked"].getSize().y)));
        window.draw(utilSprite);
    }
}

// Draw the chess pieces on the board
void Gui::drawPieces()
{
    const Board& board = game.getBoard();

	// Use absolute size to ensure pieces are always square
    const float tileSize = window.getSize().y / 10;

    bool playerWhiteColor = ("White" == game.getCurrentPlayerColor()) ? true : false;

	// Draw pieces
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            sf::Sprite pieceSprite{ pieceTextures["pawn-white"] };
            Piece p = board.getPiece(x, y);

			// Adjust drawing position based on player color
			int drawX = playerWhiteColor ? 1 + x : 8 - x;
			int drawY = playerWhiteColor ? 1 + y : 8 - y;

            if (p.type != PieceType::None)
            {
				// Determine piece name and color
                std::string pieceName = p.type == PieceType::Pawn ? "pawn" :
                    p.type == PieceType::Rook ? "rook" :
                    p.type == PieceType::Knight ? "knight" :
                    p.type == PieceType::Bishop ? "bishop" :
					p.type == PieceType::Queen ? "queen" : "king";
				std::string pieceColor = p.color == PieceColor::White ? "white" : "black";

				// If king is in check, draw check indicator under the king piece
                if (pieceName == "king" && board.isKingChecked(p.color))
                {
                    sf::Sprite utilSprite{ utilTextures["check"] };

                    utilSprite.setPosition(sf::Vector2f(drawX * tileSize, drawY * tileSize));
                    utilSprite.setScale(sf::Vector2f((tileSize / utilTextures["check"].getSize().x), (tileSize / utilTextures["check"].getSize().y)));
                    window.draw(utilSprite);
                }

                pieceSprite.setTexture(pieceTextures[pieceName + '-' + pieceColor]);
                pieceSprite.setPosition(sf::Vector2f(drawX * tileSize, drawY * tileSize));
                pieceSprite.setScale(sf::Vector2f((tileSize / pieceTextures[pieceName + '-' + pieceColor].getSize().x), (tileSize / pieceTextures[pieceName + '-' + pieceColor].getSize().y)));

                window.draw(pieceSprite);
            }
        }
    }
}