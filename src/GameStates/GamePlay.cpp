//
// Created by MLG Erwich on 18/10/2022.
//

#include "GamePlay.h"

#include <memory>

GamePlay::GamePlay(
  STATE state_id, sf::RenderWindow& game_window, sf::Font& font) :
  GameStateBase(state_id, game_window, font),
  player(new Player()),
  buffer(0.1),
  current_pause(2),
  game_over(false),
  number_of_ghosts(4),
  current_lives(MAX_NUMBER_OF_LIVES),
  current_level("level_1")
{
  ghosts.reserve(number_of_ghosts);
  for (int i = 0; i < number_of_ghosts; i++)
  {
    ghosts.emplace(ghosts.end(), std::make_unique<Ghost>());
  }

  blackboard = std::make_unique<BlackBoard>(
    *player, levels, current_level, *ghosts[0], *ghosts[1], *ghosts[2], *ghosts[3]);

  levels["level_1"] = std::make_unique<Level>(window, "Data/TileMaps/level.tmx");
  levels["level_2"] = std::make_unique<Level>(window, "Data/TileMaps/level2.tmx");
}

bool GamePlay::init()
{
  if (!life_texture.loadFromFile("Data/Images/pac man/pac man life counter.png"))
  {
    return false;
  }

  if (!player->init()) return false;

  for (auto& ghost : ghosts)
  {
    if (!ghost->init()) return false;
  }

  sf::Vector2f position = levels[current_level]->getPill((Level::getHeight() - 1) * Level::getWidth() - 2)->getSprite().getPosition();

  SFMLText(text_score, "Score: " + std::to_string(score), *font, 20,
           color, position.x / 4 + position.x / 2, position.y + 48);

  sf::Vector2f map_middle = levels[current_level]->getTileLayers()[0][(Level::getHeight() / 2 * Level::getWidth())
                                                                      + Level::getWidth() / 2]->getSprite().getPosition();

  SFMLText(text_ready, "READY?", *font, 40, color,
           map_middle.x, map_middle.y);
  SFMLText(text_go, "GO!", *font, 40, color,
           map_middle.x, map_middle.y);

  for (int i = 0; i < MAX_NUMBER_OF_LIVES; i++)
  {
    life_sprites[i] = std::make_unique<sf::Sprite>(life_texture);
    life_sprites[i]->setPosition(position.x / 4 -
                                  (1.5 * life_sprites[i]->getGlobalBounds().width) + i * 24
                                  , position.y + 48);
  }

  prepareMap();

  return true;
}

void GamePlay::prepareMap()
{
  auto spawn_points = levels[current_level]->getSpawnPoints();

  player->setSpawnPoint(spawn_points["pacman"]);
  player->restart();

  for (auto& ghost : ghosts)
  {
    ghost->setSpawnPoint(spawn_points[ghost->getName()]);
    ghost->restart();
  }
}

void GamePlay::respawn()
{
  current_lives--;
  for (auto& ghost : ghosts)
  {
    ghost->restart();
  }
  player->restart();
}

void GamePlay::reset()
{
  for (auto& ghost : ghosts)
  {
    ghost->restart();
  }
  player->restart();
  score = 0;
  current_lives = MAX_NUMBER_OF_LIVES;
  game_over = false;
  pause_timer.restart();
  current_pause = 2;

  for (auto& level : levels)
  {
    for (auto& pill : level.second->getPillLayer())
    {
      pill->setVisible(true);
    }
  }
}

void GamePlay::input(sf::Event event)
{
  player->keyHandler(event);
}

STATE GamePlay::update(float dt)
{
  if (pause_timer.getElapsedTime().asSeconds() > current_pause)
  {
    if (game_over) { return STATE::GAME_OVER; }

    std::tuple<int, int> player_pos = Level::tile(player->getMidPoint());

    player->update(dt);

    levels[current_level]->update(*player);

    for (auto& ghost : ghosts)
    {
      ghost->update(dt, *blackboard);
    }

    std::map<DIRECTIONS, int> cost;

    levels[current_level]->getSurroundingCost(player_pos, cost);

    // check if player is on a junction and then change direction of player accordingly
    bool on_junction = levels[current_level]->onJunction(*player, 6);
    if (levels[current_level]->tooLate(*player) && !player->desiredEqualsCurrent())
    {
      late_timer.restart();
    }
    if (late_timer.getElapsedTime().asSeconds() > buffer)
    {
      player->updateMovement(on_junction, cost);
    }

    // check for collision with ghosts
    for (auto& ghost : ghosts)
    {
      if (player->AABBCollision(*ghost) && !ghost->getDead())
      {
        if (ghost->getInScatterMode())
        {
          ghost->setDead(true);
          ghost->setInScatterMode(false);
          score += 200;
          text_score.setString("Score: " + std::to_string(score));
        }
        else if (!ghost->getInScatterMode())
        {
          current_pause = 1;
          pause_timer.restart();
          respawn();
        }
      }
    }

    int player_index = std::get<1>(player_pos) * Level::getWidth() + std::get<0>(player_pos);

    // get pill of tile player currently on
    auto& pill = levels[current_level]->getPill(player_index);

    if (pill->getVisible())
    {
      if (pill->getCoords() == player_pos)
      {
        pill->setVisible(false);
        score += pill->getValue();
        text_score.setString("Score: " + std::to_string(score));
        if (pill->getPower())
        {
          for (auto& ghost : ghosts)
          {
            if (!ghost->getDead()) { ghost->setInScatterMode(true); }
          }
        }
      }
    }

    for (auto& pills : levels[current_level]->getPillLayer())
    {
      if (pills->getValue() == 0) { continue; }
      if (pills->getVisible())
      {
        break;
      }
      // (26, 29) is the last pill in the array, so if it hasn't broken out the
      // for loop before then, they're all invisible (have been eaten)
      if (pills->getCoords() == std::make_tuple<int, int>(26, 29))
      {
        current_pause = 3;
        respawn();
        for (auto& pil : levels[current_level]->getPillLayer())
        {
          pil->setVisible(true);
        }
        pause_timer.restart();
        if (current_lives < MAX_NUMBER_OF_LIVES)
        {
          current_lives++;
        }

        if (current_level == "level_1") current_level = "level_2";
        else if (current_level == "level_2") current_level = "level_1";
        prepareMap();
      }
    }

    // Teleport player from 1 side of map to the other
    if (player_pos == std::make_tuple(0, 14))
    {
      player->getSprite().setPosition(26 * 16, 14 * 16);
      player->setMin();
      player->calculateMax();
    }
    else if (player_pos == std::make_tuple(27, 14))
    {
      player->getSprite().setPosition(1 * 16, 14 * 16);
      player->setMin();
      player->calculateMax();
    }

    if (current_lives <= 0)
    {
      game_over = true;
      current_pause = 1;
      pause_timer.restart();
    }
  }

  return state_id;
}

void GamePlay::render()
{
  levels[current_level]->render();
  player->render(window);

  for (auto& ghost : ghosts) { ghost->render(window); }

  for (int i = 0; i < current_lives; i++) { window.draw(*life_sprites[i]); }

  window.draw(text_score);

  if (pause_timer.getElapsedTime().asSeconds() < current_pause - 0.5)
  {
    window.draw(text_ready);
  }
  else if (pause_timer.getElapsedTime().asSeconds() < current_pause)
  {
    window.draw(text_go);
  }
}