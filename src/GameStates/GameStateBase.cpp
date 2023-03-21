//
// Created by MLG Erwich on 04/03/2022.
//

#include "GameStateBase.h"

int GameStateBase::score = 0;

GameStateBase::GameStateBase(STATE state_id, sf::RenderWindow& game_window, sf::Font &game_font) : window(game_window),
  state_id(state_id), font(new sf::Font(game_font))
{

}

bool GameStateBase::init() { return true; }

void GameStateBase::reset() {}

void GameStateBase::input(sf::Event event) {}

STATE GameStateBase::update(float dt) { return state_id; }

void GameStateBase::render() {}