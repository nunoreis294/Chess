#include <algorithm>
#include <iostream>
#include "gui.h"
#include "utils.h"

// Constructor
Gui::Gui(Game& game)
	: game(game), window(sf::VideoMode({ 900, 600 }), "Chess"), 
	  isPromotionPending(false), promotionSquare(-1, -1),
	  selectedSquare(-1, -1), selectedPiece(-1, -1), isAnalyzing(false),
	  boardFlipped(false)
{
	Utils utils;

	// Load board square textures
	squareTextures = utils.getSquareTextures();

	// Load letter textures
	letterTextures = utils.getLetterTextures();

	// Load digit textures
	digitTextures = utils.getDigitTextures();

	// Load utility textures (possible squares, attacked squares and checked)
	utilTextures = utils.getUtilTextures();

	// Load piece textures
	pieceTextures = utils.getPieceTextures();
}

// Run the GUI main loop
void Gui::run()
{
	const Board* board = game.getBoard();

	// Main loop
	while (window.isOpen())
	{
		game.updateClock();

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

				const float menuStartX = 600.f;
				const float buttonWidth = 140.f;
				const float buttonHeight = 35.f;
				const float rotateButtonX = menuStartX + 20.f;
				const float rotateButtonY = window.getSize().y - 240.f;

				if (mousePos.x >= rotateButtonX && mousePos.x <= rotateButtonX + buttonWidth &&
					mousePos.y >= rotateButtonY && mousePos.y <= rotateButtonY + buttonHeight)
				{
					boardFlipped = !boardFlipped;
					continue;
				}

				if (game.isGameOver() && !isAnalyzing)
				{
					// Layout do pop-up de fim de jogo: centraliza o overlay e os botões de análise/reinício.
					const sf::Vector2u size = window.getSize();
					const float overlayWidth = 320.f;
					const float overlayHeight = 220.f;
					const sf::Vector2f overlayPosition((size.x - overlayWidth) / 2.f, (size.y - overlayHeight) / 2.f);
					const float buttonWidth = 140.f;
					const float buttonHeight = 35.f;
					const float analyzeButtonX = overlayPosition.x + 20.f;
					const float analyzeButtonY = overlayPosition.y + 150.f;
					const float restartButtonX = overlayPosition.x + overlayWidth - 20.f - buttonWidth;
					const float restartButtonY = overlayPosition.y + 150.f;

					if (mousePos.x >= analyzeButtonX && mousePos.x <= analyzeButtonX + buttonWidth &&
						mousePos.y >= analyzeButtonY && mousePos.y <= analyzeButtonY + buttonHeight)
					{
						isAnalyzing = true;
						continue;
					}

					if (mousePos.x >= restartButtonX && mousePos.x <= restartButtonX + buttonWidth &&
						mousePos.y >= restartButtonY && mousePos.y <= restartButtonY + buttonHeight)
					{
						game.newGame();
						selectedSquare = sf::Vector2i(-1, -1);
						selectedPiece = sf::Vector2i(-1, -1);
						isPromotionPending = false;
						promotionSquare = sf::Vector2i(-1, -1);
						isAnalyzing = false;
						boardFlipped = false;
						continue;
					}

					continue;
				}

				if (isAnalyzing)
				{
					// Layout do menu lateral em modo de análise: posição dos botões de navegação e reinício.
					const float menuStartX = 600.f;
					const float buttonWidth = 140.f;
					const float buttonHeight = 35.f;
					const float previousButtonX = menuStartX + 20.f;
					const float nextButtonX = menuStartX + 20.f + buttonWidth + 10.f;
					const float restartButtonY = window.getSize().y - 180.f;
					const float restartButtonX = menuStartX + 20.f;

					if (mousePos.x >= previousButtonX && mousePos.x <= previousButtonX + buttonWidth &&
						mousePos.y >= window.getSize().y - 120.f && mousePos.y <= window.getSize().y - 120.f + buttonHeight)
					{
						game.goToPreviousMove();
						continue;
					}

					if (mousePos.x >= nextButtonX && mousePos.x <= nextButtonX + buttonWidth &&
						mousePos.y >= window.getSize().y - 120.f && mousePos.y <= window.getSize().y - 120.f + buttonHeight)
					{
						game.goToNextMove();
						continue;
					}

					if (mousePos.x >= restartButtonX && mousePos.x <= restartButtonX + buttonWidth &&
						mousePos.y >= restartButtonY && mousePos.y <= restartButtonY + buttonHeight)
					{
						game.newGame();
						selectedSquare = sf::Vector2i(-1, -1);
						selectedPiece = sf::Vector2i(-1, -1);
						isPromotionPending = false;
						promotionSquare = sf::Vector2i(-1, -1);
						isAnalyzing = false;
						boardFlipped = false;
						continue;
					}

					continue;
				}

				if (!isPromotionPending && !game.isGameOver())
				{
					const float menuStartX = 600.f;
					const float buttonWidth = 140.f;
					const float buttonHeight = 35.f;
					const float buttonX = menuStartX + 20.f;
					const float buttonY = window.getSize().y - 60.f;

					if (mousePos.x >= buttonX && mousePos.x <= buttonX + buttonWidth &&
						mousePos.y >= buttonY && mousePos.y <= buttonY + buttonHeight)
					{
						game.resignCurrentPlayer();
						selectedSquare = sf::Vector2i(-1, -1);
						selectedPiece = sf::Vector2i(-1, -1);
						isPromotionPending = false;
						promotionSquare = sf::Vector2i(-1, -1);
						continue;
					}
				}

				// Check if clicking on promotion piece selection (when promotion is pending)
				if (isPromotionPending)
				{
					// Layout do pop-up de promoção: define o espaço e a posição dos botões de escolha da peça.
					const float menuStartX = 600.f;
					const float buttonWidth = 150.f;
					const float buttonHeight = 50.f;
					const float startY = 150.f;
					const float spacing = 60.f;

					// Check which promotion piece was clicked
					if (mousePos.x >= menuStartX && mousePos.x <= menuStartX + buttonWidth)
					{
						if (mousePos.y >= startY && mousePos.y < startY + buttonHeight)
						{
							// Queen
							game.setPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1, 
										 Piece{ PieceType::Queen, board->getPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1).color });
							isPromotionPending = false;
							game.changePlayerColor();
							game.updateGameState();
							game.saveCurrentState();
							isAnalyzing = false;
						}
						else if (mousePos.y >= startY + spacing && mousePos.y < startY + spacing + buttonHeight)
						{
							// Rook
							game.setPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1, 
										 Piece{ PieceType::Rook, board->getPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1).color });
							isPromotionPending = false;
							game.changePlayerColor();
							game.updateGameState();
							game.saveCurrentState();
							isAnalyzing = false;
						}
						else if (mousePos.y >= startY + 2*spacing && mousePos.y < startY + 2*spacing + buttonHeight)
						{
							// Bishop
							game.setPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1, 
										 Piece{ PieceType::Bishop, board->getPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1).color });
							isPromotionPending = false;
							game.changePlayerColor();
							game.updateGameState();
							game.saveCurrentState();
							isAnalyzing = false;
						}
						else if (mousePos.y >= startY + 3*spacing && mousePos.y < startY + 3*spacing + buttonHeight)
						{
							// Knight
							game.setPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1, 
										 Piece{ PieceType::Knight, board->getPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1).color });
							isPromotionPending = false;
							game.changePlayerColor();
							game.updateGameState();
							game.saveCurrentState();
							isAnalyzing = false;
						}
					}
					//return;
				}

				// Calculate selected square based on mouse position
				const float tileSize = window.getSize().y / 10;
				int x = mousePos.x / tileSize;
				int y = mousePos.y / tileSize;

                if (x > 0 && x <= 8 && y > 0 && y <= 8)
                {
                    PlayerColor currentPlayerColor = game.getCurrentPlayerColor();

                    if (boardFlipped)
                    {
                        x = 9 - x;
                        y = 9 - y;
                    }

                    selectedSquare = sf::Vector2i(x, y);

                    std::cout << "x: " << x << " - y: " << y << std::endl;

                    Piece piece = board->getPiece(x - 1, y - 1);

                    std::cout << "currentPlayerColor: " << (currentPlayerColor == PlayerColor::White ? "White" : "Black") << " - piece.color: " << (piece.color == PieceColor::White ? "White" :
                        piece.color == PieceColor::Black ? "Black" : "None") << std::endl;

					// Can select piece if there is no piece currently selected or if selecting own piece
                    if ((currentPlayerColor == PlayerColor::White && piece.color == PieceColor::White) ||
                        (currentPlayerColor == PlayerColor::Black && piece.color == PieceColor::Black))
                    {
                        //std::cout << "select" << std::endl;

                        selectedPiece = selectedSquare;
                    }
					else if (selectedPiece.x != -1.f && selectedPiece.y != -1.f)
                    {
                        //std::cout << "move" << std::endl;

						// Check if the selected square is a valid move or attack for the selected piece
                        std::vector<sf::Vector2i> possibleSquares = { board->getPossibleSquares(selectedPiece) };

                        std::vector<sf::Vector2i> attackedSquares = { board->getAttackedSquares(selectedPiece) };

						possibleSquares.insert(possibleSquares.end(), attackedSquares.begin(), attackedSquares.end());

                        for (sf::Vector2i possibleSquare : possibleSquares)
                        {
							if (possibleSquare.x == selectedSquare.x && possibleSquare.y == selectedSquare.y)
							{
								// Make the move/capture
								bool success = game.makeMove((int)selectedPiece.x, (int)selectedPiece.y, (int)selectedSquare.x, (int)selectedSquare.y);

								Piece movedPiece = board->getPiece(selectedSquare.x - 1, selectedSquare.y - 1);

								if (success && movedPiece.type == PieceType::Pawn && ((currentPlayerColor == PlayerColor::White && (int)selectedSquare.y - 1 == 0) || (currentPlayerColor == PlayerColor::Black && (int)selectedSquare.y - 1 == 7)))
								{
									// Set promotion pending state
									isPromotionPending = true;
									promotionSquare = selectedSquare;
								}
								else if (success)
								{
									//Change player color if the move was successful and not pending promotion
									game.changePlayerColor();
									game.updateGameState();
									game.saveCurrentState();
									isAnalyzing = false;
								}

								break;
							}
                        }

                        selectedPiece = sf::Vector2i(-1.f, -1.f);
					}
                }
                else
                    selectedPiece = sf::Vector2i(-1.f, -1.f);

                std::cout << std::endl;
            }

			// Handle window resize events to maintain aspect ratio and scaling
            /*if (event->is<sf::Event::Resized>())
            {
                sf::View view = window.getView();

                sf::Vector2f newSize = view.getSize();

                constexpr float targetAspectRatio = 16.f / 9.f; // testing on 1920x1080
                constexpr float initialX = 1920.f, initialY = 1080.f;

                float newAspectRatio = (float)newSize.x / (float)newSize.y;

                view.setSize({ (float)newSize.x, (float)newSize.y });

                float zoomX = initialX / (float)newSize.x;
                float zoomY = initialY / (float)newSize.y;
                float zoom = (newAspectRatio > targetAspectRatio) ? zoomY : zoomX;

                view.zoom(zoom);

                window.setView(view);

                sf::View view = window.getView();

                float windowRatio = (float)window.getSize().x / (float)window.getSize().y;
                float viewRatio = view.getSize().x / (float)view.getSize().y;
                float sizeX = 1;
                float sizeY = 1;
                float posX = 0;
                float posY = 0;

                bool horizontalSpacing = true;
                if (windowRatio < viewRatio)
                    horizontalSpacing = false;

                // If horizontalSpacing is true, the black bars will appear on the left and right side.
                // Otherwise, the black bars will appear on the top and bottom.

                if (horizontalSpacing) {
                    sizeX = viewRatio / windowRatio;
                    posX = (1 - sizeX) / 2.f;
                }

                else {
                    sizeY = windowRatio / viewRatio;
                    posY = (1 - sizeY) / 2.f;
                }

                view.setViewport(sf::FloatRect(sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY)));
            }*/
        }

		// Clear the window
        window.clear(sf::Color::White);

		// Draw the board and pieces
        drawBoard();
        drawPieces();

		// Draw additional GUI elements (menu, moves, etc.)
		drawMenu();

		// Update the window
        window.display();
    }
}

// Draw the chess board (squares, letters, digits, possible moves and attacked squares)
void Gui::drawBoard()
{
	const Board* board = game.getBoard();

	// Layout do tabuleiro: usa uma grelha 10x10 para incluir as bordas laterais e os rótulos de coordenadas.
	// O tamanho de cada casa é calculado a partir da altura da janela para manter o tabuleiro quadrado.
	const float tileSize = window.getSize().y / 10;

	// Determine if the board should be displayed from the white or black perspective
	const bool playerWhiteColor = !boardFlipped;

    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            sf::Sprite boardSprite{ squareTextures["white"] };

			// Calculate position based on square size
			sf::Vector2f position(x * tileSize, y * tileSize);

			// Cores e textura das casas da borda do tabuleiro: o layout visual é definido aqui.
			if (y == 0 || y == 9 || x == 0 || x == 9)
			{
				// Casas de borda em tom castanho, usadas para o contorno visual do tabuleiro.
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
				// Casas claras (brancas) do tabuleiro principal, com variações visuais para cantos e laterais.
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
				// Casas escuras (pretas) do tabuleiro principal, com variações visuais para cantos e laterais.
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

			// Layout dos rótulos das coordenadas: letras na base e números à esquerda, com orientação conforme a cor do jogador.
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

    if (selectedPiece.x != -1.f && selectedPiece.y != -1.f)
    {
        // Draw possible squares (dot) and attacked squares (red boarder) based on selected piece
        std::vector<sf::Vector2i> possibleSquares = { board->getPossibleSquares(selectedPiece) };
        std::vector<sf::Vector2i> attackedSquares = { board->getAttackedSquares(selectedPiece) };

        sf::Sprite utilSprite{ utilTextures["dot"] };

        for (sf::Vector2i possibleSquare : possibleSquares)
        {
            const bool isAttackedSquare = std::find(attackedSquares.begin(), attackedSquares.end(), possibleSquare) != attackedSquares.end();
            if (isAttackedSquare)
                continue;

            if (playerWhiteColor)
                utilSprite.setPosition(sf::Vector2f(tileSize * possibleSquare.x, tileSize * possibleSquare.y));
            else
                utilSprite.setPosition(sf::Vector2f(tileSize * (9 - possibleSquare.x), tileSize * (9 - possibleSquare.y)));

            utilSprite.setScale(sf::Vector2f((tileSize / utilTextures["dot"].getSize().x), (tileSize / utilTextures["dot"].getSize().y)));
            window.draw(utilSprite);
        }

        utilSprite = sf::Sprite{ utilTextures["attacked"] };

        for (sf::Vector2i attackedSquare : attackedSquares)
        {
            if (playerWhiteColor)
                utilSprite.setPosition(sf::Vector2f(tileSize * attackedSquare.x, tileSize * attackedSquare.y));
            else
                utilSprite.setPosition(sf::Vector2f(tileSize * (9 - attackedSquare.x), tileSize * (9 - attackedSquare.y)));
            utilSprite.setScale(sf::Vector2f((tileSize / utilTextures["attacked"].getSize().x), (tileSize / utilTextures["attacked"].getSize().y)));
            window.draw(utilSprite);
        }
    }
}

// Draw the chess pieces on the board
void Gui::drawPieces()
{
    const Board* board = game.getBoard();

	// Use absolute size to ensure pieces are always square
    const float tileSize = window.getSize().y / 10;

    const bool playerWhiteColor = !boardFlipped;

	// Layout das peças: a posição de desenho é ajustada conforme a orientação do jogador e a marcação de xeque usa uma cor/overlay visual.
	for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            sf::Sprite pieceSprite{ pieceTextures["pawn-white"] };
            Piece p = board->getPiece(x, y);

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
                if (pieceName == "king" && board->isKingChecked(p.color))
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

// Draw additional GUI elements (menu, moves, etc.)
void Gui::drawMenu()
{
	const float menuStartX = 600.f;
	const float menuWidth = 300.f;
	const float tileSize = window.getSize().y / 10;

	const sf::Font font("Pixel.ttf");

	// Create a text
	sf::Text text(font, "hello");
	text.setCharacterSize(20);

	// Layout e cor do menu lateral: fundo claro, borda escura e área reservada para os botões e histórico.
	// Cor de fundo do painel lateral: cinza muito claro (RGB 240, 240, 240) para contrastar com o tabuleiro.
	sf::RectangleShape menuBackground(sf::Vector2f(menuWidth, window.getSize().y));
	menuBackground.setPosition(sf::Vector2f(menuStartX, 0.f));
	menuBackground.setFillColor(sf::Color(240, 240, 240));
	window.draw(menuBackground);

	// Borda do menu lateral: preta, com espessura de 2 px para separar visualmente o painel do tabuleiro.
	sf::RectangleShape menuBorder(sf::Vector2f(menuWidth, window.getSize().y));
	menuBorder.setPosition(sf::Vector2f(menuStartX, 0.f));
	menuBorder.setFillColor(sf::Color::Transparent);
	menuBorder.setOutlineColor(sf::Color::Black);
	menuBorder.setOutlineThickness(2.f);
	window.draw(menuBorder);

	// Título do menu: texto preto posicionado no canto superior esquerdo do painel lateral.
	sf::Text titleText(font, "Moves");
	titleText.setPosition(sf::Vector2f(menuStartX + 10.f, 10.f));
	titleText.setFillColor(sf::Color::Black);
	window.draw(titleText);

	// Cores dos relógios: azul para o turno ativo, cinza para o turno inativo e vermelho para tempo esgotado.
	const sf::Color activeClockColor(0, 110, 200);
	const sf::Color expiredClockColor(220, 0, 0);
	const sf::Color inactiveClockColor(60, 60, 60);

	// Relógio do jogador branco: texto posicionado logo abaixo do título, com 16 px de tamanho.
	sf::Text whiteClockText(font, "White: " + game.getFormattedTime(PlayerColor::White));
	whiteClockText.setCharacterSize(16);
	whiteClockText.setPosition(sf::Vector2f(menuStartX + 10.f, 45.f));
	whiteClockText.setFillColor(game.getRemainingTimeSeconds(PlayerColor::White) <= 0 ? expiredClockColor : (game.getCurrentPlayerColor() == PlayerColor::White && !game.isGameOver() ? activeClockColor : inactiveClockColor));
	window.draw(whiteClockText);

	// Relógio do jogador preto: texto posicionado logo abaixo do relógio branco, com o mesmo estilo visual.
	sf::Text blackClockText(font, "Black: " + game.getFormattedTime(PlayerColor::Black));
	blackClockText.setCharacterSize(16);
	blackClockText.setPosition(sf::Vector2f(menuStartX + 10.f, 70.f));
	blackClockText.setFillColor(game.getRemainingTimeSeconds(PlayerColor::Black) <= 0 ? expiredClockColor : (game.getCurrentPlayerColor() == PlayerColor::Black && !game.isGameOver() ? activeClockColor : inactiveClockColor));
	window.draw(blackClockText);

	// Layout e cor dos botões laterais: tamanho, posição e estilo visual são definidos aqui para facilitar futuras alterações.
	const float sideButtonWidth = 140.f;
	const float sideButtonHeight = 35.f;
	const float rotateButtonX = menuStartX + 20.f;
	const float rotateButtonY = window.getSize().y - 240.f;
	const float resignButtonX = menuStartX + 20.f;
	const float resignButtonY = window.getSize().y - 60.f;
	const float previousButtonX = menuStartX + 20.f;
	const float previousButtonY = window.getSize().y - 120.f;
	const float nextButtonX = menuStartX + 20.f + sideButtonWidth + 10.f;
	const float nextButtonY = window.getSize().y - 120.f;
	const float restartButtonX = menuStartX + 20.f;
	const float restartButtonY = window.getSize().y - 180.f;

	// Botão de orientação do tabuleiro: alterna entre a visão normal e a visão invertida.
	sf::RectangleShape rotateButton(sf::Vector2f(sideButtonWidth, sideButtonHeight));
	rotateButton.setPosition(sf::Vector2f(rotateButtonX, rotateButtonY));
	rotateButton.setFillColor(sf::Color(220, 220, 220));
	rotateButton.setOutlineColor(sf::Color::Black);
	rotateButton.setOutlineThickness(1.f);
	window.draw(rotateButton);

	const std::string rotateButtonLabel = boardFlipped ? "Revert" : "Rotate";
	sf::Text rotateText(font, rotateButtonLabel);
	rotateText.setCharacterSize(16);
	rotateText.setPosition(sf::Vector2f(rotateButtonX + (rotateButtonLabel == "Revert" ? 38.f : 40.f), rotateButtonY + 8.f));
	rotateText.setFillColor(sf::Color::Black);
	window.draw(rotateText);

	if (!isAnalyzing)
	{
		// Botão de resign: fundo cinza claro (220,220,220) com borda preta e texto preto para um contraste limpo.
		sf::RectangleShape resignButton(sf::Vector2f(sideButtonWidth, sideButtonHeight));
		resignButton.setPosition(sf::Vector2f(resignButtonX, resignButtonY));
		resignButton.setFillColor(sf::Color(220, 220, 220));
		resignButton.setOutlineColor(sf::Color::Black);
		resignButton.setOutlineThickness(1.f);
		window.draw(resignButton);

		// Texto do botão de resign: centralizado visualmente dentro do retângulo do botão.
		sf::Text resignText(font, "Resign");
		resignText.setCharacterSize(16);
		resignText.setPosition(sf::Vector2f(resignButtonX + 42.f, resignButtonY + 8.f));
		resignText.setFillColor(sf::Color::Black);
		window.draw(resignText);
	}

	if (isAnalyzing || game.isGameOver())
	{
		// Botões de navegação e reinício: usam o mesmo esquema visual de cinza claro com borda preta.
		sf::RectangleShape previousButton(sf::Vector2f(sideButtonWidth, sideButtonHeight));
		previousButton.setPosition(sf::Vector2f(previousButtonX, previousButtonY));
		previousButton.setFillColor(sf::Color(220, 220, 220));
		previousButton.setOutlineColor(sf::Color::Black);
		previousButton.setOutlineThickness(1.f);
		window.draw(previousButton);

		// Texto do botão Previous alinhado ligeiramente à direita do centro do botão.
		sf::Text previousText(font, "Previous");
		previousText.setCharacterSize(16);
		previousText.setPosition(sf::Vector2f(previousButtonX + 25.f, previousButtonY + 8.f));
		previousText.setFillColor(sf::Color::Black);
		window.draw(previousText);

		sf::RectangleShape nextButton(sf::Vector2f(sideButtonWidth, sideButtonHeight));
		nextButton.setPosition(sf::Vector2f(nextButtonX, nextButtonY));
		nextButton.setFillColor(sf::Color(220, 220, 220));
		nextButton.setOutlineColor(sf::Color::Black);
		nextButton.setOutlineThickness(1.f);
		window.draw(nextButton);

		// Texto do botão Next posicionado mais à direita para equilibrar o layout horizontal.
		sf::Text nextText(font, "Next");
		nextText.setCharacterSize(16);
		nextText.setPosition(sf::Vector2f(nextButtonX + 45.f, nextButtonY + 8.f));
		nextText.setFillColor(sf::Color::Black);
		window.draw(nextText);

		sf::RectangleShape restartButton(sf::Vector2f(sideButtonWidth, sideButtonHeight));
		restartButton.setPosition(sf::Vector2f(restartButtonX, restartButtonY));
		restartButton.setFillColor(sf::Color(220, 220, 220));
		restartButton.setOutlineColor(sf::Color::Black);
		restartButton.setOutlineThickness(1.f);
		window.draw(restartButton);

		// Texto do botão Restart alinhado ao centro do botão, acima dos botões de navegação.
		sf::Text restartText(font, "Restart");
		restartText.setCharacterSize(16);
		restartText.setPosition(sf::Vector2f(restartButtonX + 40.f, restartButtonY + 8.f));
		restartText.setFillColor(sf::Color::Black);
		window.draw(restartText);
	}

	Board* board = game.getBoard();

	std::vector<Move> moveHistory = board->getMoveHistory();
	// Draw move history (show last 8 moves)
	float moveY = 50.f;
	int startIndex = std::max(0, (int)moveHistory.size() - 8);

	for (int i = startIndex; i < (int)moveHistory.size(); ++i)
	{
		std::string moveText = moveHistory[i].notation;
		sf::Text moveTextObj(font, moveText);
		moveTextObj.setCharacterSize(14);
		moveTextObj.setPosition(sf::Vector2f(menuStartX + 10.f, moveY));
		moveTextObj.setFillColor(sf::Color::Black);
		window.draw(moveTextObj);
		moveY += 25.f;
	}

	if (!isAnalyzing && game.isGameOver())
	{
		// Pop-up de fim de jogo: overlay escurecido a 160 de alpha para destacar o painel central.
		sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
		overlay.setFillColor(sf::Color(0, 0, 0, 160));
		window.draw(overlay);

		// Caixa do pop-up central: 320x220 px, fundo branco e borda preta para destacar o conteúdo.
		sf::RectangleShape dialogBackground(sf::Vector2f(320.f, 220.f));
		dialogBackground.setPosition(sf::Vector2f((window.getSize().x - 320.f) / 2.f, (window.getSize().y - 220.f) / 2.f));
		dialogBackground.setFillColor(sf::Color(255, 255, 255));
		dialogBackground.setOutlineColor(sf::Color::Black);
		dialogBackground.setOutlineThickness(2.f);
		window.draw(dialogBackground);

		// Título do pop-up de fim de jogo: posicionamento centralizado na caixa de diálogo.
		GameResult result = game.getGameResult();
		std::string title = result.type == GameResultType::Checkmate ? "Checkmate" : result.type == GameResultType::Resignation ? "Resignation" : result.type == GameResultType::Draw ? "Draw" : "Analysis";
		sf::Text resultTitle(font, title);
		resultTitle.setCharacterSize(24);
		resultTitle.setPosition(sf::Vector2f((window.getSize().x - 120.f) / 2.f, (window.getSize().y - 220.f) / 2.f + 25.f));
		resultTitle.setFillColor(sf::Color::Black);
		window.draw(resultTitle);

		sf::Text resultMessage(font, isAnalyzing ? "Use the side menu to review the moves." : (result.message.empty() ? "Review the game move by move" : result.message));
		resultMessage.setCharacterSize(18);
		resultMessage.setPosition(sf::Vector2f((window.getSize().x - 240.f) / 2.f, (window.getSize().y - 220.f) / 2.f + 70.f));
		resultMessage.setFillColor(sf::Color::Black);
		window.draw(resultMessage);

		if (!isAnalyzing)
		{
			const float buttonWidth = 140.f;
			const float buttonHeight = 35.f;
			const float analyzeButtonX = (window.getSize().x - 320.f) / 2.f + 20.f;
			const float analyzeButtonY = (window.getSize().y - 220.f) / 2.f + 150.f;
			const float restartButtonX = (window.getSize().x - 320.f) / 2.f + 160.f;
			const float restartButtonY = (window.getSize().y - 220.f) / 2.f + 150.f;

			sf::RectangleShape analyzeButton(sf::Vector2f(buttonWidth, buttonHeight));
			analyzeButton.setPosition(sf::Vector2f(analyzeButtonX, analyzeButtonY));
			analyzeButton.setFillColor(sf::Color(220, 220, 220));
			analyzeButton.setOutlineColor(sf::Color::Black);
			analyzeButton.setOutlineThickness(1.f);
			window.draw(analyzeButton);

			sf::Text analyzeText(font, "Analyze");
			analyzeText.setCharacterSize(16);
			analyzeText.setPosition(sf::Vector2f(analyzeButtonX + 38.f, analyzeButtonY + 8.f));
			analyzeText.setFillColor(sf::Color::Black);
			window.draw(analyzeText);

			sf::RectangleShape restartButton(sf::Vector2f(buttonWidth, buttonHeight));
			restartButton.setPosition(sf::Vector2f(restartButtonX, restartButtonY));
			restartButton.setFillColor(sf::Color(220, 220, 220));
			restartButton.setOutlineColor(sf::Color::Black);
			restartButton.setOutlineThickness(1.f);
			window.draw(restartButton);

			sf::Text restartText(font, "Restart");
			restartText.setCharacterSize(16);
			restartText.setPosition(sf::Vector2f(restartButtonX + 42.f, restartButtonY + 8.f));
			restartText.setFillColor(sf::Color::Black);
			window.draw(restartText);
		}
	}

	// Draw promotion piece selection menu if promotion is pending
	if (isPromotionPending)
	{
		// Pop-up de promoção: overlay escurecido e painel central com os botões de escolha da peça.
		// A cor e o posicionamento do painel podem ser alterados sem mexer na lógica do jogo.
		// Overlay de fundo: preto semi-transparente com alpha 100 para escurecer o tabuleiro.
		sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
		overlay.setFillColor(sf::Color(0, 0, 0, 100));
		window.draw(overlay);

		// Caixa do pop-up de promoção: 250x300 px, branca e alinhada ao painel lateral para destacar as opções.
		sf::RectangleShape dialogBackground(sf::Vector2f(250.f, 300.f));
		dialogBackground.setPosition(sf::Vector2f(menuStartX + 25.f, 100.f));
		dialogBackground.setFillColor(sf::Color(255, 255, 255));
		dialogBackground.setOutlineColor(sf::Color::Black);
		dialogBackground.setOutlineThickness(2.f);
		window.draw(dialogBackground);

		// Draw promotion title
		sf::Text promotionTitle(font, "Select Piece");
        promotionTitle.setCharacterSize(14);
		promotionTitle.setPosition(sf::Vector2f(menuStartX + 40.f, 115.f));
		promotionTitle.setFillColor(sf::Color::Black);
		window.draw(promotionTitle);

		// Draw promotion piece options
		const float buttonWidth = 150.f;
		const float buttonHeight = 50.f;
		const float startY = 150.f;
		const float spacing = 60.f;
		const sf::Color buttonColor(200, 200, 200);
		const sf::Color buttonHoverColor(150, 150, 150);

		std::vector<std::string> promotionPieces = { "Queen", "Rook", "Bishop", "Knight" };

		for (int i = 0; i < 4; ++i)
		{
			sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
			button.setPosition(sf::Vector2f(menuStartX + 50.f, startY + i * spacing));
			button.setFillColor(buttonColor);
			button.setOutlineColor(sf::Color::Black);
			button.setOutlineThickness(1.f);
			window.draw(button);

			sf::Text buttonText(font, promotionPieces[i]);
            buttonText.setCharacterSize(16);
			buttonText.setPosition(sf::Vector2f(menuStartX + 65.f, startY + i * spacing + 12.f));
			buttonText.setFillColor(sf::Color::Black);
			window.draw(buttonText);
		}
	}
}
