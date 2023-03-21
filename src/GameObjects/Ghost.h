//
// Created by MLG Erwich on 19/11/2022.
//

#ifndef PACMANSFML_GHOST_H
#define PACMANSFML_GHOST_H

#include "Entity.h"
#include "../Other/Pathfinding.h"
#include "../Map/Level.h"
#include "../BehaviourTrees/BehaviourTree.h"

class Ghost : public Entity
{
 public:
  Ghost();
  ~Ghost() = default;

  bool init() override;

  std::string getName();

  void restart();

  void update(float dt, BlackBoard& blackboard);

 private:
  static int ghost_num;

  float start_time;
  sf::Clock start_timer;

  std::unique_ptr<BehaviourTree> behaviour_tree;

  std::string name;

  sf::IntRect scatter_rect;
  sf::IntRect dead_rect;
};

#endif // PACMANSFML_GHOST_H
