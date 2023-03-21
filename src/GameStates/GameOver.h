//
// Created by MLG Erwich on 18/10/2022.
//

#ifndef PACMANSFML_GAMEOVER_H
#define PACMANSFML_GAMEOVER_H

#include "GameStateBase.h"
#include "../Other/TextSetup.h"

class GameOver : public GameStateBase
{
 public:
  GameOver(STATE state_id, sf::RenderWindow& game_window, sf::Font& font);
  ~GameOver() = default;

  bool init() override;
  void reset() override;
  void input(sf::Event event) override;
  STATE update(float dt) override;
  void render() override;

 private:
  sf::Text title;
  sf::Text score_text;
  sf::Text to_menu;

  STATE transition;
};

#endif // PACMANSFML_GAMEOVER_H
