//
// Created by perwi on 28/10/2022.
//

#ifndef PACMANSFML_PLAYER_H
#define PACMANSFML_PLAYER_H

#include "GameObject.h"
#include "Entity.h"
#include <cmath>
#include <iostream>
#include <map>

class Player : public Entity
{
 public:
  Player();
  ~Player() = default;

  bool init() override;

  bool desiredEqualsCurrent();

  void restart();

  void updateMovement(bool on_junction, std::map<DIRECTIONS, int> &cost);

  void updateFacing();

  void keyHandler(sf::Event event);
  void update(float dt) override;
 private:
  sf::IntRect sprite_rect;

  DIRECTIONS desired_direction;
  std::map<DIRECTIONS, sf::Vector2f> desired_movement;
};

#endif // PACMANSFML_PLAYER_H
