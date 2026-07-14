#include <iostream>
#include "gui.h"
#include "utils.h"

// Constructor
Gui::Gui(Game& game)
	: game(game), window(sf::VideoMode({ 900, 600 }), "Chess"), 
	  isPromotionPending(false), promotionSquare(-1, -1),
	  selectedSquare(-1, -1), selectedPiece(-1, -1)
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

				if (game.isGameOver())
				{
					const sf::Vector2u size = window.getSize();
					const float buttonWidth = 150.f;
					const float buttonHeight = 40.f;
					const sf::Vector2f buttonPosition(size.x / 2.f - buttonWidth / 2.f, size.y / 2.f + 70.f);

					if (mousePos.x >= buttonPosition.x && mousePos.x <= buttonPosition.x + buttonWidth &&
						mousePos.y >= buttonPosition.y && mousePos.y <= buttonPosition.y + buttonHeight)
					{
						game.newGame();
						selectedSquare = sf::Vector2i(-1, -1);
						selectedPiece = sf::Vector2i(-1, -1);
						isPromotionPending = false;
						promotionSquare = sf::Vector2i(-1, -1);
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
						}
						else if (mousePos.y >= startY + spacing && mousePos.y < startY + spacing + buttonHeight)
						{
							// Rook
							game.setPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1, 
										 Piece{ PieceType::Rook, board->getPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1).color });
							isPromotionPending = false;
							game.changePlayerColor();
							game.updateGameState();
						}
						else if (mousePos.y >= startY + 2*spacing && mousePos.y < startY + 2*spacing + buttonHeight)
						{
							// Bishop
							game.setPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1, 
										 Piece{ PieceType::Bishop, board->getPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1).color });
							isPromotionPending = false;
							game.changePlayerColor();
							game.updateGameState();
						}
						else if (mousePos.y >= startY + 3*spacing && mousePos.y < startY + 3*spacing + buttonHeight)
						{
							// Knight
							game.setPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1, 
										 Piece{ PieceType::Knight, board->getPiece((int)promotionSquare.x - 1, (int)promotionSquare.y - 1).color });
							isPromotionPending = false;
							game.changePlayerColor();
							game.updateGameState();
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

                    if (currentPlayerColor == PlayerColor::Black)
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

	// Use absolute size to ensure squares are always square
	// 8 rows + 2 for borders
    const float tileSize = window.getSize().y / 10;

	// Determine if player is white or black to adjust board orientation
    bool playerWhiteColor = (PlayerColor::White == game.getCurrentPlayerColor()) ? true : false;

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

    if (selectedPiece.x != -1.f && selectedPiece.y != -1.f)
    {
        // Draw possible squares (dot) and attacked squares (red boarder) based on selected piece
        std::vector<sf::Vector2i> possibleSquares = { board->getPossibleSquares(selectedPiece) };

        sf::Sprite utilSprite{ utilTextures["dot"] };

        for (sf::Vector2i possibleSquare : possibleSquares)
        {
            if (playerWhiteColor)
                utilSprite.setPosition(sf::Vector2f(tileSize * possibleSquare.x, tileSize * possibleSquare.y));
            else
                utilSprite.setPosition(sf::Vector2f(tileSize * (9 - possibleSquare.x), tileSize * (9 - possibleSquare.y)));

            utilSprite.setScale(sf::Vector2f((tileSize / utilTextures["dot"].getSize().x), (tileSize / utilTextures["dot"].getSize().y)));
            window.draw(utilSprite);
        }

        std::vector<sf::Vector2i> attackedSquares = { board->getAttackedSquares(selectedPiece) };

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

    bool playerWhiteColor = (PlayerColor::White == game.getCurrentPlayerColor()) ? true : false;

	// Draw pieces
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

	// Draw menu background
	sf::RectangleShape menuBackground(sf::Vector2f(menuWidth, window.getSize().y));
	menuBackground.setPosition(sf::Vector2f(menuStartX, 0.f));
	menuBackground.setFillColor(sf::Color(240, 240, 240));
	window.draw(menuBackground);

	// Draw menu border
	sf::RectangleShape menuBorder(sf::Vector2f(menuWidth, window.getSize().y));
	menuBorder.setPosition(sf::Vector2f(menuStartX, 0.f));
	menuBorder.setFillColor(sf::Color::Transparent);
	menuBorder.setOutlineColor(sf::Color::Black);
	menuBorder.setOutlineThickness(2.f);
	window.draw(menuBorder);

	// Draw title
	sf::Text titleText(font, "Moves");
titleText.setPosition(sf::Vector2f(menuStartX + 10.f, 10.f));
titleText.setFillColor(sf::Color::Black);
	window.draw(titleText);

	const float resignButtonWidth = 140.f;
	const float resignButtonHeight = 35.f;
	const float resignButtonX = menuStartX + 20.f;
	const float resignButtonY = window.getSize().y - 60.f;

	sf::RectangleShape resignButton(sf::Vector2f(resignButtonWidth, resignButtonHeight));
	resignButton.setPosition(sf::Vector2f(resignButtonX, resignButtonY));
	resignButton.setFillColor(sf::Color(220, 220, 220));
	resignButton.setOutlineColor(sf::Color::Black);
	resignButton.setOutlineThickness(1.f);
	window.draw(resignButton);

	sf::Text resignText(font, "Resign");
	resignText.setCharacterSize(16);
	resignText.setPosition(sf::Vector2f(resignButtonX + 42.f, resignButtonY + 8.f));
	resignText.setFillColor(sf::Color::Black);
	window.draw(resignText);

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

	if (game.isGameOver())
	{
		sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
		overlay.setFillColor(sf::Color(0, 0, 0, 160));
		window.draw(overlay);

		sf::RectangleShape dialogBackground(sf::Vector2f(320.f, 220.f));
		dialogBackground.setPosition(sf::Vector2f((window.getSize().x - 320.f) / 2.f, (window.getSize().y - 220.f) / 2.f));
		dialogBackground.setFillColor(sf::Color(255, 255, 255));
		dialogBackground.setOutlineColor(sf::Color::Black);
		dialogBackground.setOutlineThickness(2.f);
		window.draw(dialogBackground);

		GameResult result = game.getGameResult();
		std::string title = result.type == GameResultType::Checkmate ? "Checkmate" : result.type == GameResultType::Resignation ? "Resignation" : "Draw";
		sf::Text resultTitle(font, title);
		resultTitle.setCharacterSize(24);
		resultTitle.setPosition(sf::Vector2f((window.getSize().x - 120.f) / 2.f, (window.getSize().y - 220.f) / 2.f + 25.f));
		resultTitle.setFillColor(sf::Color::Black);
		window.draw(resultTitle);

		sf::Text resultMessage(font, result.message);
		resultMessage.setCharacterSize(18);
		resultMessage.setPosition(sf::Vector2f((window.getSize().x - 110.f) / 2.f, (window.getSize().y - 220.f) / 2.f + 70.f));
		resultMessage.setFillColor(sf::Color::Black);
		window.draw(resultMessage);

		sf::RectangleShape restartButton(sf::Vector2f(150.f, 40.f));
		restartButton.setPosition(sf::Vector2f(window.getSize().x / 2.f - 75.f, window.getSize().y / 2.f + 70.f));
		restartButton.setFillColor(sf::Color(200, 200, 200));
		restartButton.setOutlineColor(sf::Color::Black);
		restartButton.setOutlineThickness(1.f);
		window.draw(restartButton);

		sf::Text restartText(font, "Restart");
		restartText.setCharacterSize(16);
		restartText.setPosition(sf::Vector2f(window.getSize().x / 2.f - 45.f, window.getSize().y / 2.f + 80.f));
		restartText.setFillColor(sf::Color::Black);
		window.draw(restartText);
	}

	// Draw promotion piece selection menu if promotion is pending
	if (isPromotionPending)
	{
		// Draw semi-transparent overlay
		sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
		overlay.setFillColor(sf::Color(0, 0, 0, 100));
		window.draw(overlay);

		// Draw promotion dialog background
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
