#include <iostream>
#include "gui.h"

// Constructor: initialize the window and any resources
Gui::Gui(Game& game)
    : game(game), window(sf::VideoMode({ 900, 600 }), "Chess")
{
    const std::string squareColors[] = { "white", "black", "brown" };
    const std::string squareTypes[] = { "", "-side", "-corner"};

    for (const auto& color : squareColors)
    {
        for (const auto& type : squareTypes)
        {
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

    // List of piece names (assuming you have these images)
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

// Main loop: handle events and draw the board
void Gui::run()
{
	const Board& board = game.getBoard();

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);

        drawBoard();
        drawPieces();

        window.display();
    }
}

// Draw the chessboard squares
void Gui::drawBoard()
{
    sf::Font font;
    if (!font.openFromFile("text.ttf"))
    {
        // error...
    }
    //sf::Text pieceText(font, "", 15);

    const float tileSize = window.getSize().y / 10;

    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            sf::Sprite boardSprite{ squareTextures["white"] };

			sf::Vector2f position(x * tileSize, y * tileSize);

            //pieceText.setFillColor(sf::Color::Black);
            //pieceText.setPosition(position);

            if (y == 0 || y == 9 || x == 0 || x == 9)
            {
                if (y == x || (x == 0 && y == 9) || (x == 9 && y == 0))
                {
                    //pieceText.setString("c " + std::to_string(x) + " " + std::to_string(y));

                    boardSprite.setTexture(squareTextures["brown-corner"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["brown-corner"].getSize().x), (tileSize / squareTextures["brown-corner"].getSize().y)));
                }
                else
                {
                    //pieceText.setString("s " + std::to_string(x) + " " + std::to_string(y));

                    boardSprite.setTexture(squareTextures["brown-side"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["brown-side"].getSize().x), (tileSize / squareTextures["brown-side"].getSize().y)));

                    if (y == 0 || y == 9)
                    {
                        boardSprite.setOrigin(sf::Vector2f(0.f, 32.f));
                        boardSprite.setRotation(sf::degrees(90.f));
                    }
                }
            }            
            else if ((x + y) % 2 == 0)
            {
                if ((x == 1 && y == 1) || (x == 8 && y == 8))
                {
                    //pieceText.setString("c " + std::to_string(x) + " " + std::to_string(y));

                    boardSprite.setTexture(squareTextures["white-corner"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["white-corner"].getSize().x), (tileSize / squareTextures["white-corner"].getSize().y)));

                    if (x == 8 && y == 8)
                    {
                        boardSprite.setOrigin(sf::Vector2f(32.f, 32.f));
                        boardSprite.setRotation(sf::degrees(180.f));
                    }
                }
                else if (y == 1 || y == 8 || x == 1 || x == 8)
                {
                    //pieceText.setString("s " + std::to_string(x) + " " + std::to_string(y));

                    boardSprite.setTexture(squareTextures["white-side"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["white-side"].getSize().x), (tileSize / squareTextures["white-side"].getSize().y)));

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
                    //pieceText.setString("" + std::to_string(x) + " " + std::to_string(y));

                    boardSprite.setTexture(squareTextures["white"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["white"].getSize().x), (tileSize / squareTextures["white"].getSize().y)));
                }
            }
            else
            {
                if ((x == 1 && y == 8) || (x == 8 && y == 1))
                {
                    //pieceText.setString("c " + std::to_string(x) + " " + std::to_string(y));

                    boardSprite.setTexture(squareTextures["black-corner"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["black-corner"].getSize().x), (tileSize / squareTextures["black-corner"].getSize().y)));

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
                    //pieceText.setString("s " + std::to_string(x) + " " + std::to_string(y));

                    boardSprite.setTexture(squareTextures["black-side"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["black-side"].getSize().x), (tileSize / squareTextures["black-side"].getSize().y)));

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
                    //pieceText.setString("" + std::to_string(x) + " " + std::to_string(y));

                    boardSprite.setTexture(squareTextures["black"]);
                    boardSprite.setPosition(position);
                    boardSprite.setScale(sf::Vector2f((tileSize / squareTextures["black"].getSize().x), (tileSize / squareTextures["black"].getSize().y)));
                }
            }

            window.draw(boardSprite);

            //window.draw(pieceText);
        }
    }

    // Replace the problematic line with the following:
    bool playerWhiteColor = ("White" == game.getCurrentPlayerColor()) ? true : false;

    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            const std::string letters[] = { "a", "b", "c", "d", "e", "f", "g", "h" };
            const std::string digits[] = { "1", "2", "3", "4", "5", "6", "7", "8" };

            sf::Sprite letterDigitSprite{ letterTextures["a"] };

            sf::Vector2f position(x * tileSize, y * tileSize);

            if ((playerWhiteColor && ((x == 0 (y != 0 && y != 9)) || (y == 9 && (x != 0 && x != 9)))) || (!playerWhiteColor && ((y == 0 && (x != 0 && x != 9)) || (x == 9 && (x != 0 && x != 9)))))
            {
                if ((playerWhiteColor && y == 9 && (x != 0 && x != 9)) || (!playerWhiteColor && y == 0 && (x != 0 && x != 9)))
                {
					std::string letter = letters[(x - 1)];

                    letterDigitSprite.setTexture(letterTextures[letter]);
                    letterDigitSprite.setPosition(position);
                    letterDigitSprite.setScale(sf::Vector2f((tileSize / letterTextures[letter].getSize().x), (tileSize / letterTextures[letter].getSize().y)));
                }
                else if ((playerWhiteColor && x == 0 && (y != 0 && y != 9)) || (!playerWhiteColor && x == 9 && (x != 0 && x != 9)))
                {
                    std::string digit = digits[(8 - y)];

                    letterDigitSprite.setTexture(digitTextures[digit]);
                    letterDigitSprite.setPosition(position);
                    letterDigitSprite.setScale(sf::Vector2f((tileSize / digitTextures[digit].getSize().x), (tileSize / digitTextures[digit].getSize().y)));
                }

                window.draw(letterDigitSprite);
            }
        }
    }
}

// Draw the chess pieces (placeholder: you would use textures/sprites in a real app)
void Gui::drawPieces()
{
    // Example: draw simple circles for pieces
    // In a real app, load and draw piece images
    const Board& board = game.getBoard();
    const float tileSize = window.getSize().y / 10;

    sf::Font font;
    if (!font.openFromFile("text.ttf"))
    {
        // error...
    }

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            Piece p = board.getPiece(x, y);

            if (p.type != PieceType::None)
            {
				sf::Text pieceText(font ,"", 24);
                sf::CircleShape pieceShape(tileSize / 2 - 10);

                char c = ' ';

                if (p.type != PieceType::None)
                {
                    c = p.type == PieceType::Pawn ? 'P' :
                        p.type == PieceType::Rook ? 'R' :
                        p.type == PieceType::Knight ? 'N' :
                        p.type == PieceType::Bishop ? 'B' :
                        p.type == PieceType::Queen ? 'Q' : 'K';

                    if (p.color == PieceColor::Black)
                    {
                        c = std::tolower(c);
                    }
                }

                pieceText.setString(c);
				pieceText.setFillColor(p.color == PieceColor::White ? sf::Color::Black : sf::Color::White);
				pieceText.setPosition(sf::Vector2f((1 + x) * tileSize + 10, (1 + y) * tileSize + 10));

                pieceShape.setPosition(sf::Vector2f((1 + x) * tileSize + 10, (1 + y) * tileSize + 10));
                pieceShape.setFillColor(p.color == PieceColor::White ? sf::Color::White : sf::Color::Black);

                window.draw(pieceShape);
                window.draw(pieceText);
            }
        }
    }
}