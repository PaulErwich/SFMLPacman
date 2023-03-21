//
// Created by MLG Erwich on 18/10/2022.
//

#include "GameOver.h"

GameOver::GameOver(
  STATE state_id, sf::RenderWindow& game_window, sf::Font& font) :
  GameStateBase(state_id, game_window, font),
  transition(state_id)
{
}

bool GameOver::init()
{
  SFMLText(title, "GAME OVER", *font, 40, color,
           window.getSize().x / 2, window.getSize().y / 8);

  SFMLText(score_text, "FINAL SCORE: " + std::to_string(score), *font, 30, color,
           window.getSize().x / 2, window.getSize().y / 2);

  SFMLText(to_menu, "ENTER TO RETURN\nTO MENU", *font, 30, color,
           window.getSize().x / 2, (window.getSize().y / 4) * 3);

  return true;
}

void GameOver::reset()
{
  transition = state_id;
  SFMLText(score_text, "FINAL SCORE: " + std::to_string(score), *font, 30, color,
           window.getSize().x / 2, window.getSize().y / 2);
  std::cout << "restart\n";
}

void GameOver::input(sf::Event event)
{
  if (event.type == sf::Event::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::Enter)
    {
      transition = STATE::GAME_MENU;
    }
  }
}

STATE GameOver::update(float dt)
{
  if (transition != state_id)
  {
    return transition;
  }
  return state_id;
}

void GameOver::render()
{
  window.draw(title);
  window.draw(score_text);
  window.draw(to_menu);
}