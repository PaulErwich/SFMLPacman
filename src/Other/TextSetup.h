//
// Created by MLG Erwich on 10/11/2022.
//

#ifndef PACMANSFML_TEXTSETUP_H
#define PACMANSFML_TEXTSETUP_H

#include <SFML/Graphics.hpp>

void SFMLText(sf::Text& text, const std::string& string, sf::Font& font,
                     int char_size, sf::Color color, float x, float y);

#endif // PACMANSFML_TEXTSETUP_H
