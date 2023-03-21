
#include "Game.h"
#include <iostream>

using namespace tinyxml2;

Game::Game(sf::RenderWindow& game_window)
  : window(game_window),
  current_state(STATE::GAME_MENU)
{
  srand(time(NULL));

  font = std::make_unique<sf::Font>();
  font->loadFromFile("Data/Fonts/OpenSans-Bold.ttf");

  game_states.insert(std::make_pair(STATE::GAME_MENU, std::make_unique<GameMenu>(STATE::GAME_MENU, window, *font)));
  game_states.insert(std::make_pair(STATE::GAME_PLAY, std::make_unique<GamePlay>(STATE::GAME_PLAY, window, *font)));
  game_states.insert(std::make_pair(STATE::GAME_OVER, std::make_unique<GameOver>(STATE::GAME_OVER, window, *font)));
}

bool Game::init()
{
  for (auto& state: game_states)
  {
    if (!state.second->init()) { return false;}
  }

  return true;
}

void Game::update(float dt)
{
  STATE prev_state = current_state;
  current_state = game_states[current_state]->update(dt);

  if (current_state != prev_state)
  {
    if (current_state == STATE::GAME_EXIT)
    {
      window.close();
      return;
    }
    else
    {
      game_states[current_state]->reset();
    }
  }
}

void Game::render()
{
  game_states[current_state]->render();
}

void Game::input(sf::Event event)
{
  game_states[current_state]->input(event);
}