#pragma once

#include "game.h"
#include <map>

/**
 * @file utils.h
 * @brief Utility helper functions for loading textures and resources.
 */

class Utils {
public:
    /** Load and return square textures used by the GUI. */
    std::map<std::string, sf::Texture> getSquareTextures();

    /** Load and return letter textures (a-h). */
    std::map<std::string, sf::Texture> getLetterTextures();

    /** Load and return digit textures (1-8). */
    std::map<std::string, sf::Texture> getDigitTextures();

    /** Load and return piece textures. */
    std::map<std::string, sf::Texture> getPieceTextures();

    /** Load and return utility textures (dots, highlights, etc.). */
    std::map<std::string, sf::Texture> getUtilTextures();
};
