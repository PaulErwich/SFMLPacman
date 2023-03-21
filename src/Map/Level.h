//
// Created by MLG Erwich on 19/10/2022.
//

#ifndef PACMANSFML_LEVEL_H
#define PACMANSFML_LEVEL_H

#include "../../tinyxml2/tinyxml2.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <tuple>
#include "Tile.h"
#include "PillTile.h"
#include "../GameObjects/Entity.h"

class Level
{
 private:
  sf::RenderWindow &window;
  tinyxml2::XMLDocument file;

  static const int WIDTH = 28;
  static const int HEIGHT = 31;
  static const int TILE_LAYER_NUMBER = 2;
  static const int TOTAL_LAYERS = 3;

  sf::Texture layer_texture;

  int csv_layers[TOTAL_LAYERS][WIDTH * HEIGHT];
  int cost_map[WIDTH * HEIGHT];
  std::array<std::array<std::unique_ptr<Tile>, WIDTH * HEIGHT>, TILE_LAYER_NUMBER> tile_layers;
  std::array<std::unique_ptr<PillTile>, WIDTH * HEIGHT> pill_layer;

  std::vector<sf::Vector2f> junction_coords;
  std::vector<std::tuple<int, int>> junction_tiles;

  std::map<std::string, sf::Vector2f> spawn_points;

 public:
  Level(sf::RenderWindow &game_window, const char* level_path);
  ~Level() = default;

  bool loadLevel(const char* level_path);
  bool textureLevel();
  void loadObjects();

  bool onJunction(GameObject& player, float _offset);
  bool tooLate(GameObject& player);

  static sf::Vector2i world(std::tuple<int, int> tile_coords);
  static std::tuple<int, int> tile(sf::Vector2f point);

  void update(Entity& player);
  void render();

  static int getWidth();
  static int getHeight();

  int getTileCost(int x, int y);
  void getSurroundingCost(std::tuple<int, int> tile_to_check,
                                               std::map<DIRECTIONS, int>& cost);

  const std::array<
    std::array<std::unique_ptr<Tile>, WIDTH * HEIGHT>, TILE_LAYER_NUMBER>&
  getTileLayers() const;

  const std::array<std::unique_ptr<PillTile>, WIDTH * HEIGHT>& getPillLayer() const;

  const std::unique_ptr<PillTile>& getPill(int index) const;

  const std::map<std::string, sf::Vector2f>& getSpawnPoints() const;
};

#endif // PACMANSFML_LEVEL_H
