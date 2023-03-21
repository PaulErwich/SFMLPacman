//
// Created by MLG Erwich on 18/10/2022.
//

#ifndef PACMANSFML_GAMEMENU_H
#define PACMANSFML_GAMEMENU_H

#include "GameStateBase.h"
#include "../Other/TextSetup.h"

class GameMenu : public GameStateBase
{
 public:
  GameMenu(STATE state_id, sf::RenderWindow& game_window, sf::Font& game_font);
  ~GameMenu() = default;

  bool init() override;

  void reset() override;
  void input(sf::Event event) override;
  STATE update(float dt) override;
  void render() override;

 private:
  sf::Text title;
  sf::Text play;

  STATE transition;
};

#endif // PACMANSFML_GAMEMENU_H
