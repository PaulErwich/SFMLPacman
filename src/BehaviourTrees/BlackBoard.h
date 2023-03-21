//
// Created by MLG Erwich on 20/11/2022.
//

#ifndef PACMANSFML_BLACKBOARD_H
#define PACMANSFML_BLACKBOARD_H

#include "../GameObjects/Entity.h"
#include "../Map/Level.h"
#include <iostream>

struct BlackBoard
{
  BlackBoard(Entity& _player, std::map<std::string, std::unique_ptr<Level>>& _maps,
             std::string& _current_level, Entity& _blinky,
             Entity& _pinky, Entity& _inky, Entity& _clyde) :
    player(_player), maps(_maps), current_map(_current_level),
    blinky(_blinky), pinky(_pinky),
  inky(_inky), clyde(_clyde) {}

  Entity& player;

  std::map<std::string, std::unique_ptr<Level>>& maps;
  std::string& current_map;

  Entity& blinky;
  Entity& pinky;
  Entity& inky;
  Entity& clyde;

  // Store tree ghost end pos
  std::tuple<int, int> ghost_end_pos;
};

#endif // PACMANSFML_BLACKBOARD_H
