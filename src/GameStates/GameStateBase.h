//
// Created by MLG Erwich on 14/10/2022.
//

#ifndef PACMANSFML_GAMESTATEBASE_H
#define PACMANSFML_GAMESTATEBASE_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum class STATE
{
  GAME_UNKNOWN = -1,
  GAME_EXIT = 0,
  GAME_MENU = 1,
  GAME_PLAY = 2,
  GAME_OVER = 5
};

class GameStateBase
{
 public:
  GameStateBase(STATE state_id, sf::RenderWindow& game_window, sf::Font &game_font);
  ~GameStateBase() = default;
  virtual bool init();
  virtual void reset();
  virtual void input(sf::Event event);
  virtual STATE update(float dt);
  virtual void render();

 protected:
  STATE state_id;
  sf::RenderWindow& window;

  std::unique_ptr<sf::Font> font;
  static int score;

  const sf::Color color = sf::Color::White;
};

#endif // PACMANSFML_GAMESTATEBASE_H
