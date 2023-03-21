//
// Created by MLG Erwich on 05/11/2022.
//

#include "Entity.h"

Entity::Entity() :
  SPEED(100),
  in_scatter_mode(false),
  facing(Facing::EAST),
  dead(false),
  scatter_time(5)
{

}

void Entity::setDestination(std::vector<sf::Vector2i> path)
{
  destination = path;

  for (auto& tile: destination)
  {
    tile.x -= width / 2;
    tile.y -= height / 2;

  }

  setDirection();
}

std::vector<sf::Vector2i> Entity::getDestination() { return destination; }

void Entity::setDirection()
{
  if (!destination.empty())
  {
    if (destination[0].x == min.x &&
        destination[0].y == min.y) return;

    direction.x = destination[0].x - min.x;
    direction.y = destination[0].y - min.y;
    float normalise = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x /= normalise;
    direction.y /= normalise;
  }
}

void Entity::setSpawnPoint(sf::Vector2f new_spawn) { spawn_point = new_spawn; }
sf::Vector2f Entity::getSpawnPoint() { return spawn_point; }

void Entity::setInScatterMode(bool mode) { in_scatter_mode = mode; scatter_timer.restart(); }
bool Entity::getInScatterMode() const { return in_scatter_mode; }

sf::Vector2f Entity::getScatterPoint() { return scatter_point; }

Facing Entity::getFacing() { return facing; }

sf::Vector2f Entity::getMidPoint() { return { min.x + width / 2, min.y + height / 2 }; }

bool Entity::getDead() const { return dead; }
void Entity::setDead(bool _dead) { dead = _dead; }