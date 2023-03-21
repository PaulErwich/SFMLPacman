
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>
#include "ResourceHandler.h"
#include "../tinyxml2/tinyxml2.h"

#include "Map/Level.h"
#include "GameObjects/Player.h"

#include "Other/Pathfinding.h"

#include "GameStates/GameStateBase.h"
#include "GameStates/GameMenu.h"
#include "GameStates/GamePlay.h"
#include "GameStates/GameOver.h"
#include <map>

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game() = default;
  bool init();
  void update(float dt);
  void render();
  void input(sf::Event event);

 private:
  sf::RenderWindow& window;
  std::unique_ptr<sf::Font> font;

  std::map<STATE, std::unique_ptr<GameStateBase>> game_states;
  STATE current_state;
};

#endif // PLATFORMER_GAME_H
