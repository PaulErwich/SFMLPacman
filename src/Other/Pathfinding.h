//
// Created by p2-erwich on 04/11/2022.
//

#ifndef PACMANSFML_PATHFINDING_H
#define PACMANSFML_PATHFINDING_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <tuple>
#include <queue>
#include <unordered_map>
#include "../GameObjects/GameObject.h"
#include "../Map/Level.h"

using Location = std::tuple<int, int>;

namespace std
{
  template<> struct hash<Location>
  {
    std::size_t operator()(const Location& id) const noexcept
    {
      return std::get<0>(id) ^ std::get<1>(id);
    }
  };
}

using PQElement = std::pair<int, Location>;

std::vector<sf::Vector2i> pathfinding(sf::Vector2f xy, Level& map, GameObject& entity);

void AStar(Level& map, Location& start, Location& end,
           std::unordered_map<Location, Location>& came_from,
           std::unordered_map<Location, int>& cost_so_far);

std::vector<Location> getNeighbours(Level& map, Location start);

struct PriorityQueue
{
  std::priority_queue<PQElement, std::vector<PQElement>,
    std::greater<PQElement>> elements;

  inline bool empty() const {
    return elements.empty();
  }

  inline void put(Location item, int priority)
  {
    elements.emplace(priority, item);
  }

  Location get()
  {
    Location best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

int heuristic(Location& a, Location& b);

std::vector<Location> reconstruct_path(
  Location start, Location end,
  std::unordered_map<Location, Location>& came_from);

#endif // PACMANSFML_PATHFINDING_H
