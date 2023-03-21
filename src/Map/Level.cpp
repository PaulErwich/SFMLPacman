//
// Created by MLG Erwich on 19/10/2022.
//

#include "Level.h"
#include <memory>

Level::Level(sf::RenderWindow& game_window, const char* level_path) : window(game_window)
{
  for (int & i : cost_map)
  {
    i = 0;
  }

  junction_coords.reserve(70);
  junction_tiles.reserve(70);

  loadLevel(level_path);
  textureLevel();

  loadObjects();
}

bool Level::loadLevel(const char* level_path)
{
  if (file.LoadFile(level_path) != tinyxml2::XML_SUCCESS)
  {
    std::cout << "File didn't load" << std::endl;
    return false;
  }

  tinyxml2::XMLNode* root = file.RootElement();
  tinyxml2::XMLElement* element;

  const char* csv;
  std::string temp;

  for (int i = 0; i < TOTAL_LAYERS; i++)
  {
    const char* layer_char;
    int layer_cost;
    element = root->FirstChildElement("layer");

    for (int k = 0; k < i; k++)
    {
      element = element->NextSiblingElement("layer");
      std::cout << "here" << std::endl;
    }

    layer_char = element->FirstChildElement("properties")->FirstChildElement("property")->Attribute("value");
    std::cout << layer_char << std::endl;
    layer_cost = std::atoi(layer_char);
    std::cout << layer_cost << std::endl;

    int l = 1;
    for (int j = 0; j < WIDTH * HEIGHT; j++)
    {
      csv = element->FirstChildElement("data")->GetText();

      temp = csv;

      if (temp[l] == '\n')
      {
        l++;
      }

      if (j == 27 && i == 1)
      {
        std::cout << "hi" << std::endl;
      }

      if (temp[l + 1] != '\n' && temp[l + 1] != ',')
      {
        csv_layers[i][j] = ((int(temp[l]) - 48) * 10) + (int(temp[l + 1]) - 48);
        l += 3;
      }
      else
      {
        csv_layers[i][j] = int(temp[l]) - 48;
        l += 2;
      }

      if (csv_layers[i][j] != 0)
      {
        cost_map[j] += layer_cost;
      }
    }
  }
  cost_map[12 * WIDTH + 13] = 0;
  cost_map[12 * WIDTH + 14] = 0;

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      std::cout << cost_map[i * WIDTH + j] << " ";
    }
    std::cout << std::endl;
  }

  csv = nullptr;
  delete csv;

  return true;
}

bool Level::textureLevel()
{
  int texture_number = 0;

  if (!layer_texture.loadFromFile("Data/Images/pac man/tiles set.png"))
  {
    std::cout << "rip\n";
    return false;
  }

  for (int i = 0; i < TILE_LAYER_NUMBER; i++)
  {
    for (int j = 0; j < HEIGHT; j++)
    {
      for (int k = 0; k < WIDTH; k++)
      {
        texture_number = csv_layers[i][j * WIDTH + k] - 1;

        tile_layers[i][j * WIDTH + k] = std::make_unique<Tile>(layer_texture, k, j, texture_number, cost_map[j * WIDTH + k]);
      }
    }
  }

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      texture_number = csv_layers[2][i * WIDTH + j] - 1;
      pill_layer[i * WIDTH + j] = std::make_unique<PillTile>(layer_texture, j, i, texture_number, cost_map[i * WIDTH + j]);
    }
  }

  return true;
}

void Level::loadObjects()
{
  tinyxml2::XMLNode* root = file.RootElement();

  tinyxml2::XMLElement* element = root->FirstChildElement("objectgroup");
  tinyxml2::XMLElement* current = element->FirstChildElement("object");

  // load in all objects from the Junctions object layer in tmx
  bool children = true;
  int i = 0;
  while(children)
  {
    junction_coords[i] = {current->FloatAttribute("x"), current->FloatAttribute("y")};

    junction_tiles.emplace(junction_tiles.end(), tile(junction_coords[i]));

    if (current->NextSiblingElement("object") == nullptr)
    {
      children = false;
      break;
    }

    current = current->NextSiblingElement("object");
    i++;
  }

  // store them & set boolean within tile to true where necessary
  for (auto & junction_tile : junction_tiles)
  {
    int x, y, index;
    x = std::get<0>(junction_tile);
    y = std::get<1>(junction_tile);

    index = y * WIDTH + x;

    tile_layers[1][index]->setJunction(true);
  }

  element = element->NextSiblingElement("objectgroup");
  current = element->FirstChildElement("object");

  // load objects from SpawnPoints object layer in tmx
  children = true;
  while(children)
  {
    spawn_points[current->Attribute("name")] = {current->FloatAttribute("x"), current->FloatAttribute("y")};
    if (current->NextSiblingElement("object") == nullptr)
    {
      children = false;
      break;
    }
    current = current->NextSiblingElement("object");
  }
}

bool Level::onJunction(GameObject& player, float _offset)
{
  sf::Vector2f movement = player.getMovement();
  sf::Vector2f player_mid = {player.getMin().x + player.getWidth() / 2,
                              player.getMin().y + player.getHeight() / 2};
  sf::Vector2f player_offset = {player_mid};

  float offset = _offset;
  if (movement.x != 0)
  {
    if (movement.x > 0) { player_offset.x -= offset; }
    else { player_offset.x += offset; }
  }
  if (movement.y != 0)
  {
    if (movement.y > 0) { player_offset.y -= offset; }
    else { player_offset.y += offset; }
  }

  std::tuple<int, int> current_pos = tile(player_offset);

  int x, y, number;
  x = std::get<0>(current_pos);
  y = std::get<1>(current_pos);
  number = y * WIDTH + x;

  if (tile_layers[1][number]->getJunction())
  {
    return true;
  }

  return false;
}

bool Level::tooLate(GameObject& player)
{
  if (onJunction(player, 0))
  {
    sf::Vector2f movement = player.getMovement();
    sf::Vector2f player_mid = {player.getMin().x + player.getWidth() / 2,
                                player.getMin().y + player.getHeight() / 2};
    sf::Vector2i tile_pos = world(tile(player_mid));

    bool too_late = false;

    if (movement.x != 0)
    {
      if (movement.x > 0)
      {
        if (player_mid.x > tile_pos.x) { too_late = true; }
      }
      else
      {
        if (player_mid.x < tile_pos.x) { too_late = true; }
      }
    }
    if (movement.y != 0)
    {
      if (movement.y > 0)
      {
        if (player_mid.y > tile_pos.y) { too_late = true; }
      }
      else
      {
        if (player_mid.y < tile_pos.y) { too_late = true; }
      }
    }
    if (too_late) { return true; }
  }

  return false;
}

sf::Vector2i Level::world(std::tuple<int, int> tile_coords)
{
  return sf::Vector2i {(std::get<0>(tile_coords) + 1) * 16 - (16 / 2),
                      ( std::get<1>(tile_coords) + 1) * 16 - (16 / 2)};
}

std::tuple<int, int> Level::tile(sf::Vector2f point)
{
  return {int(point.x / 16),
           int(point.y / 16)};
}

void Level::update(Entity& player)
{
  std::tuple<int, int> player_pos = tile({player.getMin().x + 8, player.getMin().y + 8});

  int player_index = std::get<1>(player_pos) * WIDTH + std::get<0>(player_pos);

  for (int i = 0; i < HEIGHT; i++)
  {
    for (int j = 0; j < WIDTH; j++)
    {
      int index = i * WIDTH + j;
      float distance = std::sqrt(std::pow(player.getMin().x - tile_layers[1][index]->getMin().x, 2) +
                                 std::pow(player.getMin().y - tile_layers[1][index]->getMin().y, 2));

      if (distance < 80)
      {
        if (player.AABBCollision(*tile_layers[1][index]))
        {
          player.getSprite().setPosition(tile_layers[0][player_index]->getSprite().getPosition());
          player.setMin();
          player.calculateMax();
          player.setMovement({0, 0});
        }
      }
    }
  }
}

void Level::render()
{
  for (auto& layer : tile_layers)
  {
    for (auto& tile : layer)
    {
      if (tile->getVisible())
      {
        window.draw(tile->getSprite());
      }
    }
  }
  for (auto& pill : pill_layer)
  {
    if (pill->getVisible())
    {
      window.draw(pill->getSprite());
    }
  }
}

int Level::getWidth() { return WIDTH; }
int Level::getHeight() { return HEIGHT; }

int Level::getTileCost(int x, int y) { return cost_map[y * WIDTH + x]; }

void Level::getSurroundingCost(std::tuple<int, int> tile_to_check, std::map<DIRECTIONS, int> &cost)
{
  cost[DIRECTIONS::LEFT] = getTileCost(std::get<0>(tile_to_check) - 1,
                                       std::get<1>(tile_to_check));
  cost[DIRECTIONS::RIGHT] = getTileCost(std::get<0>(tile_to_check) + 1,
                                        std::get<1>(tile_to_check));
  cost[DIRECTIONS::UP] = getTileCost(std::get<0>(tile_to_check),
                                     std::get<1>(tile_to_check) - 1);
  cost[DIRECTIONS::DOWN] = getTileCost(std::get<0>(tile_to_check),
                                       std::get<1>(tile_to_check) + 1);
}

const std::array<
  std::array<std::unique_ptr<Tile>, Level::WIDTH * Level::HEIGHT>, Level::TILE_LAYER_NUMBER>&
Level::getTileLayers() const
{
  return tile_layers;
}

const std::array<std::unique_ptr<PillTile>, Level::WIDTH * Level::HEIGHT>&
Level::getPillLayer() const
{
  return pill_layer;
}

const std::unique_ptr<PillTile>& Level::getPill(int index) const
{ return pill_layer[index]; }

const std::map<std::string, sf::Vector2f>& Level::getSpawnPoints() const
{
  return spawn_points;
}
