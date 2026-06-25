#pragma once

#include "game.h"
#include <map>

class Utils {
public:
    std::map<std::string, sf::Texture> getSquareTextures();
    std::map<std::string, sf::Texture> getLetterTextures();
    std::map<std::string, sf::Texture> getDigitTextures();
    std::map<std::string, sf::Texture> getPieceTextures();
    std::map<std::string, sf::Texture> getUtilTextures();
};