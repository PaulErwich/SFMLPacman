//
// Created by MLG Erwich on 20/11/2022.
//

#ifndef PACMANSFML_GHOSTNODES_H
#define PACMANSFML_GHOSTNODES_H

#include "TreeNodes.h"
#include "../Other/Pathfinding.h"

class CheckDestination : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    if (actor.getDestination().size() < 10)
    {
      return ReturnType::FAILURE;
    }
    return ReturnType::SUCCESS;
  }
};

class PathToPacman : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    actor.setDestination(pathfinding(blackBoard.player.getMidPoint(),
                                     *blackBoard.maps[blackBoard.current_map], actor));
    return ReturnType::SUCCESS;
  }
};

class InScatterMode : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackboard) override
  {
    if (actor.getInScatterMode())
    {
      return ReturnType::SUCCESS;
    }

    return ReturnType::FAILURE;
  }
};

class PathToScatterPoint : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackboard) override
  {
    actor.setDestination(pathfinding(actor.getScatterPoint(),
                                     *blackboard.maps[blackboard.current_map], actor));

    return ReturnType::SUCCESS;
  }
};

class PathToFuturePacman : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    Facing player_direction = blackBoard.player.getFacing();
    std::tuple<int, int> current_pos = Level::tile(blackBoard.player.getMidPoint());
    sf::Vector2i end_pos;
    switch (player_direction)
    {
      case Facing::NORTH:
        // Original game bug apparently
        // If Pac-Man faces up - it's 2 up and to the left
        end_pos = Level::world({std::get<0>(current_pos) - 2,
                                         std::get<1>(current_pos) - 2});
        break;
      case Facing::SOUTH:
        end_pos = Level::world({std::get<0>(current_pos),
                                         std::get<1>(current_pos) + 2});
        break;
      case Facing::EAST:
        end_pos = Level::world({std::get<0>(current_pos) + 2,
                                         std::get<1>(current_pos)});
        break;
      case Facing::WEST:
        end_pos = Level::world({std::get<0>(current_pos) - 2,
                                         std::get<1>(current_pos)});
        break;
      default:
        break;
    }

    sf::Vector2f end_posF = {float(end_pos.x), float(end_pos.y)};

    actor.setDestination(pathfinding(end_posF,
                                     *blackBoard.maps[blackBoard.current_map], actor));

    return ReturnType::SUCCESS;
  }
};

class CheckFutureCost : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    Facing player_direction = blackBoard.player.getFacing();
    std::tuple<int, int> current_pos = Level::tile(blackBoard.player.getMidPoint());

    switch (player_direction)
    {
      case Facing::NORTH:
        if (blackBoard.maps[blackBoard.current_map]->getTileCost(std::get<0>(current_pos), std::get<1>(current_pos) + 2) > 0)
          return ReturnType::FAILURE;
        break;
      case Facing::SOUTH:
        if (blackBoard.maps[blackBoard.current_map]->getTileCost(std::get<0>(current_pos), std::get<1>(current_pos) - 2) > 0)
          return ReturnType::FAILURE;
        break;
      case Facing::EAST:
        if (blackBoard.maps[blackBoard.current_map]->getTileCost(std::get<0>(current_pos) + 2, std::get<1>(current_pos)) > 0)
          return ReturnType::FAILURE;
        break;
      case Facing::WEST:
        if (blackBoard.maps[blackBoard.current_map]->getTileCost(std::get<0>(current_pos) - 2, std::get<1>(current_pos)) > 0)
          return ReturnType::FAILURE;
        break;
      default:
        break;
    }
    return ReturnType::SUCCESS;
  }
};

class FindDistance : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    std::tuple<int, int> blinky_pos = Level::tile(blackBoard.blinky.getMidPoint());
    sf::Vector2f pinky_going_to;
    if (!blackBoard.pinky.getDestination().empty())
    {
      pinky_going_to = {float(blackBoard.pinky.getDestination().back().x), float(blackBoard.pinky.getDestination().back().y)};
    }
    else { pinky_going_to = {float(Level::world(Level::tile(blackBoard.player.getMidPoint())).x),
      float(Level::world(Level::tile(blackBoard.player.getMidPoint())).y)}; }
    std::tuple<int, int> pinky_end_pos = Level::tile(pinky_going_to);

    int x_diff = std::get<0>(pinky_end_pos) - std::get<0>(blinky_pos);
    int y_diff = std::get<1>(pinky_end_pos) - std::get<1>(blinky_pos);

    std::tuple<int, int> inky_end_pos = {std::get<0>(pinky_end_pos) + x_diff,
                                      std::get<1>(pinky_end_pos) + y_diff};

    if (std::get<0>(inky_end_pos) < 0 or std::get<0>(inky_end_pos) > Level::getWidth() - 1)
    {
      blackBoard.ghost_end_pos = inky_end_pos;
      return ReturnType::FAILURE;
    }
    if (std::get<1>(inky_end_pos) < 0 or std::get<1>(inky_end_pos) > Level::getHeight() - 1)
    {
      blackBoard.ghost_end_pos = inky_end_pos;
      return ReturnType::FAILURE;
    }

    if (blackBoard.maps[blackBoard.current_map]->getTileCost(std::get<0>(inky_end_pos), std::get<1>(inky_end_pos)) > 0)
    {
      blackBoard.ghost_end_pos = inky_end_pos;
      return ReturnType::FAILURE;
    }

    blackBoard.ghost_end_pos = inky_end_pos;
    return ReturnType::SUCCESS;
  }
};

class MoveEndPointInMap : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    int x = std::get<0>(blackBoard.ghost_end_pos);
    int y = std::get<1>(blackBoard.ghost_end_pos);

    if (x < 0) { x = 1; }
    else if (x > Level::getWidth() - 1) { x = Level::getWidth() - 2; }

    if (y < 0) { y = 1; }
    else if (y > Level::getHeight() - 1) { y = Level::getHeight() - 2; }

    blackBoard.ghost_end_pos = { x, y };

    return ReturnType::SUCCESS;
  }
};

class MakeEndPointValid : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    int x = std::get<0>(blackBoard.ghost_end_pos);
    int y = std::get<1>(blackBoard.ghost_end_pos);

    if (blackBoard.maps[blackBoard.current_map]->getTileCost(x, y) < 1)
    { return ReturnType::SUCCESS; }

    while (blackBoard.maps[blackBoard.current_map]->getTileCost(x, y) < 1)
    {
      if (x < Level::getWidth() / 2)
      { x++; }
      else if (x >= Level::getWidth() / 2)
      { x--; }

      if (blackBoard.maps[blackBoard.current_map]->getTileCost(x, y) < 1) break;

      if (y < Level::getHeight() / 2)
      { y++; }
      else if (y >= Level::getHeight() / 2)
      { y--; }
    }

    return ReturnType::SUCCESS;
  }
};

class InkyPath : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    sf::Vector2f end_point = {
      float(Level::world(blackBoard.ghost_end_pos).x),
    float(Level::world(blackBoard.ghost_end_pos).y)};
    actor.setDestination(pathfinding(end_point, *blackBoard.maps[blackBoard.current_map], actor));

    return ReturnType::SUCCESS;
  }
};

class WithinRange : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    std::tuple<int, int> pacman_pos = Level::tile(blackBoard.player.getMidPoint());
    std::tuple<int, int> ghost_pos = Level::tile(actor.getMidPoint());

    if (abs(std::get<0>(pacman_pos) - std::get<0>(ghost_pos)) +
        abs(std::get<1>(pacman_pos) - std::get<1>(ghost_pos)) <= 8)
    {
      return ReturnType::SUCCESS;
    }

    return ReturnType::FAILURE;
  }
};

class FarAway : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    std::tuple<int, int> pacman_pos = Level::tile(blackBoard.player.getMidPoint());
    std::tuple<int, int> ghost_pos = Level::tile(actor.getMidPoint());

    if (abs(std::get<0>(pacman_pos) - std::get<0>(ghost_pos)) +
        abs(std::get<1>(pacman_pos) - std::get<1>(ghost_pos)) >= 16)
    {
      return ReturnType::SUCCESS;
    }

    return ReturnType::SUCCESS;
  }
};

class GhostDead : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    if (actor.getDead())
    {
      return ReturnType::SUCCESS;
    }

    return ReturnType::FAILURE;
  }
};

class PathToSpawnPoint : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    actor.setDestination(pathfinding(actor.getSpawnPoint(), *blackBoard.maps[blackBoard.current_map], actor));
    return ReturnType::SUCCESS;
  }
};

class AtSpawnPoint : public Leaf
{
 public:
  ReturnType tick(Entity& actor, BlackBoard& blackBoard) override
  {
    if (Level::tile(actor.getMidPoint()) ==
        Level::tile({actor.getSpawnPoint().x + 8, actor.getSpawnPoint().y + 8}))
    {
      actor.setDead(false);
      return ReturnType::SUCCESS;
    }
    return ReturnType::FAILURE;
  }
};

#endif // PACMANSFML_GHOSTNODES_H
