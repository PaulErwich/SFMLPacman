//
// Created by MLG Erwich on 18/10/2022.
//

#include "GameMenu.h"

GameMenu::GameMenu(
  STATE state_id, sf::RenderWindow& game_window, sf::Font& game_font) :
  GameStateBase(state_id, game_window, game_font),
  transition(state_id)
{
}

bool GameMenu::init()
{
  SFMLText(title, "PACMAN", *font, 40, color,
           window.getSize().x / 2, window.getSize().y / 8);

  SFMLText(play, "ENTER TO PLAY", *font, 40, color,
           window.getSize().x / 2, window.getSize().y / 2);

  return true;
}

void GameMenu::reset()
{
  transition = state_id;
  std::cout << "restart\n";
}

void GameMenu::input(sf::Event event)
{
  if (event.type == sf::Event::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::Enter)
    {
      transition = STATE::GAME_PLAY;
    }
  }
}

STATE GameMenu::update(float dt)
{
  if (transition != state_id)
  {
    return transition;
  }

  return state_id;
}

void GameMenu::render()
{
  window.draw(title);
  window.draw(play);
}
