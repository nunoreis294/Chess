#include "utils.h"

std::map<std::string, sf::Texture> Utils::getSquareTextures()
{
	std::map<std::string, sf::Texture> squareTextures;

    // Load board square textures
    const std::string squareColors[] = { "white", "black", "brown" };
    const std::string squareTypes[] = { "", "-side", "-corner" };

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

	return squareTextures;
}

std::map<std::string, sf::Texture> Utils::getLetterTextures()
{
    std::map<std::string, sf::Texture> letterTextures;

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

	return letterTextures;
}

std::map<std::string, sf::Texture> Utils::getDigitTextures()
{
    std::map<std::string, sf::Texture> digitTextures;

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

	return digitTextures;
}

std::map<std::string, sf::Texture> Utils::getPieceTextures()
{
    std::map<std::string, sf::Texture> pieceTextures;

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

	return pieceTextures;
}

std::map<std::string, sf::Texture> Utils::getUtilTextures()
{
	std::map<std::string, sf::Texture> utilTextures;

    // Load utility textures (possible squares, attacked squares and checked)
    const std::string utilNames[] = { "dot", "attacked", "check" };

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

	return utilTextures;
}
