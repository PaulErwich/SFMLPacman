//
// Created by MLG Erwich on 19/11/2022.
//

#include "Ghost.h"

int Ghost::ghost_num = 1;

Ghost::Ghost() : behaviour_tree(new BlinkyTree()), start_time(0)
{
  behaviour_tree->buildTree();
  SPEED = 80;
  sprite_rect = {1, 1, 16, 16};
  scatter_rect = {69, 18, 16, 16};
  dead_rect = {86, 18, 16, 16};
}

bool Ghost::init()
{
  std::string ghost_texture;

  switch (ghost_num)
  {
    case 1:
      ghost_texture = "Data/Images/pac man/ghost_red.png";
      name = "blinky";
      scatter_point = {26 * 16, 1 * 16};
      break;
    case 2:
      ghost_texture = "Data/Images/pac man/ghost_pink.png";
      name = "pinky";
      scatter_point = {1 * 16, 1 * 16};
      behaviour_tree = std::make_unique<PinkyTree>();
      behaviour_tree->buildTree();
      break;
    case 3:
      ghost_texture = "Data/Images/pac man/ghost_blue.png";
      name = "inky";
      scatter_point = {26 * 16, 29 * 16};
      behaviour_tree = std::make_unique<InkyTree>();
      behaviour_tree->buildTree();
      break;
    case 4:
      ghost_texture = "Data/Images/pac man/ghost_orange.png";
      name = "clyde";
      scatter_point = {1 * 16, 29 * 16};
      behaviour_tree = std::make_unique<ClydeTree>();
      behaviour_tree->buildTree();
      break;
  }

  start_time = (ghost_num - 1) * 7.5f;
  ghost_num++;

  if (!texture->loadFromFile(ghost_texture)) { return false; }

  sprite->setTexture(*texture);
  sprite->setTextureRect(sprite_rect);
  sprite->setPosition(spawn_point);

  width = sprite->getGlobalBounds().width;
  height = sprite->getGlobalBounds().height;
  setMin();
  calculateMax();

  return true;
}

std::string Ghost::getName() { return name; }

void Ghost::restart()
{
  destination.clear();
  start_timer.restart();
  dead = false;
  in_scatter_mode = false;
  sprite->setTextureRect(sprite_rect);
  sprite->setPosition(spawn_point);
  setMin();
  calculateMax();
}

void Ghost::update(float dt, BlackBoard& blackboard)
{
  if (start_timer.getElapsedTime().asSeconds() > start_time)
  {
    behaviour_tree->run(*this, blackboard);

    if (!destination.empty())
    {
      float pos = abs((min.x - destination[0].x) +
                      (min.y - destination[0].y));

      if (pos < SPEED * 0.02)
      {
        sprite->setPosition(destination[0].x, destination[0].y);
        setMin();
        calculateMax();
        destination.erase(destination.begin());
        setDirection();
      }
      sprite->move(direction.x * SPEED * dt,
                   direction.y * SPEED * dt);
    }

    if (in_scatter_mode && scatter_timer.getElapsedTime().asSeconds() > scatter_time)
    {
      in_scatter_mode = false;
    }

    if (in_scatter_mode)
    {
      sprite->setTextureRect(scatter_rect);
      SPEED = 50;
    }
    else if (dead)
    {
      sprite->setTextureRect(dead_rect);
      SPEED = 100;
    }
    else
    {
      sprite->setTextureRect(sprite_rect);
      SPEED = 80;
    }
  }
  else
  {
    in_scatter_mode = false;
    dead = false;
  }

  setMin();
  calculateMax();
}
