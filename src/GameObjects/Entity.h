//
// Created by MLG Erwich on 05/11/2022.
//

#ifndef PACMANSFML_ENTITY_H
#define PACMANSFML_ENTITY_H

#include "GameObject.h"
#include <cmath>

enum class Facing
{
  NONE = 0,
  NORTH = 1,
  SOUTH = 2,
  EAST = 3,
  WEST = 4
};

class Entity : public GameObject
{
 public:
  Entity();
  ~Entity() = default;

  void setDestination(std::vector<sf::Vector2i> path);
  std::vector<sf::Vector2i> getDestination();
  void setDirection();

  void setSpawnPoint(sf::Vector2f new_spawn);
  sf::Vector2f getSpawnPoint();

  void setInScatterMode(bool mode);
  bool getInScatterMode() const;

  sf::Vector2f getScatterPoint();

  Facing getFacing();


  sf::Vector2f getMidPoint();

  bool getDead() const;
  void setDead(bool _dead);

 protected:
  sf::IntRect sprite_rect;

  float SPEED;
  std::vector<sf::Vector2i> destination;
  sf::Vector2f direction;

  sf::Vector2f spawn_point;

  bool in_scatter_mode;
  sf::Vector2f scatter_point;

  float scatter_time;
  sf::Clock scatter_timer;

  Facing facing;

  bool dead;
};

#endif // PACMANSFML_ENTITY_H
