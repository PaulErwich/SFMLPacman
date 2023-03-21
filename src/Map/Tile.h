//
// Created by MLG Erwich on 26/10/2022.
//

#ifndef PACMANSFML_TILE_H
#define PACMANSFML_TILE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <tuple>
#include "../GameObjects/GameObject.h"

class Tile : public GameObject
{
 public:
  Tile(sf::Texture &_texture, int x, int y, int texture_num, int _cost);
  ~Tile() = default;

  // get X, Y coordinates of the tile
  std::tuple<int, int> getCoords();

  // get cost of tile
  int getCost() const;
  // set boolean for if tile is a junction
  void setJunction(bool _junction);
  // get boolean for if tile is a junction
  bool getJunction() const;

 protected:
  // Represents X, Y of the tile
  // This is for pathfinding
  std::tuple<int, int> tile_coords;
  int cost;

  bool junction;
};

#endif // PACMANSFML_TILE_H
