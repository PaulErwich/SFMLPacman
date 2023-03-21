//
// Created by MLG Erwich on 18/10/2022.
//

#ifndef PACMANSFML_GAMEPLAY_H
#define PACMANSFML_GAMEPLAY_H

#include <cmath>

#include "GameStateBase.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Ghost.h"
#include "../Map/Level.h"
#include "../Other/Pathfinding.h"
#include "../Other/TextSetup.h"
#include "../BehaviourTrees/BlackBoard.h"

class GamePlay : public GameStateBase
{
 public:
  GamePlay(STATE state_id, sf::RenderWindow& game_window, sf::Font& font);
  ~GamePlay() = default;

  bool init() override;

  void prepareMap();

  void respawn();
  void reset() override;
  void input(sf::Event event) override;
  STATE update(float dt) override;
  void render() override;

 private:
  std::map<std::string, std::unique_ptr<Level>> levels;
  std::string current_level;

  std::unique_ptr<Player> player;

  std::unique_ptr<BlackBoard> blackboard;

  std::vector<std::unique_ptr<Ghost>> ghosts;
  int number_of_ghosts;

  sf::Clock late_timer;
  float buffer;

  sf::Text text_score;

  static const int MAX_NUMBER_OF_LIVES = 5;
  int current_lives;
  sf::Texture life_texture;
  std::array<std::unique_ptr<sf::Sprite>, MAX_NUMBER_OF_LIVES> life_sprites;

  sf::Clock pause_timer;
  float current_pause;
  bool game_over;

  sf::Text text_ready;
  sf::Text text_go;
};

#endif // PACMANSFML_GAMEPLAY_H
