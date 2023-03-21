//
// Created by perwi on 28/10/2022.
//

#include "Player.h"

Player::Player() : desired_direction(DIRECTIONS::NONE),
  sprite_rect(sf::IntRect(0, 0, 32, 32))
{
  desired_movement[DIRECTIONS::NONE] = {0, 0};
  desired_movement[DIRECTIONS::LEFT] = {-1, 0};
  desired_movement[DIRECTIONS::RIGHT] = {1, 0};
  desired_movement[DIRECTIONS::UP] = {0, -1};
  desired_movement[DIRECTIONS::DOWN] = {0, 1};
}

bool Player::init()
{
  if (!texture->loadFromFile("Data/Images/pac man/pacman.png"))
  {
    return false;
  }

  sprite->setTexture(*texture);
  sprite->setTextureRect(sprite_rect);
  sprite->setScale(.5, .5);
  sprite->setPosition(spawn_point);

  width = sprite->getGlobalBounds().width;
  height = sprite->getGlobalBounds().height;

  setMin();
  calculateMax();

  return true;
}

bool Player::desiredEqualsCurrent()
{
  if (desired_movement[desired_direction] == movement) { return true; }
  if (movement == sf::Vector2f{0, 0}) { return true; }
  return false;
}

void Player::restart()
{
  movement = {0, 0};

  destination.clear();

  sprite->setPosition(spawn_point);
  setMin();
  calculateMax();
}

void Player::updateMovement(bool on_junction, std::map<DIRECTIONS, int> &cost)
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
  {
    std::cout << "hi\n";
  }

  if (on_junction)
  {
    if (movement.x == 0 and movement.y == 0) { movement = desired_movement[desired_direction]; }

    if (movement.x != 0)
    {
      switch(desired_direction)
      {
        case DIRECTIONS::UP:
          if (cost[DIRECTIONS::UP] > 0) { return; }
          break;
        case DIRECTIONS::DOWN:
          if (cost[DIRECTIONS::DOWN] > 0) { return; }
          break;
        default:
          break;
      }
      if (desired_movement[desired_direction].y != 0) { movement = desired_movement[desired_direction]; }
    }
    else if (movement.y != 0)
    {
      switch(desired_direction)
      {
        case DIRECTIONS::LEFT:
          if (cost[DIRECTIONS::LEFT] > 0) { return; }
          break;
        case DIRECTIONS::RIGHT:
          if (cost[DIRECTIONS::RIGHT] > 0) { return; }
          break;
        default:
          break;
      }
      if (desired_movement[desired_direction].x != 0) { movement = desired_movement[desired_direction]; }
    }

    return;
  }
  else
  {
    if (movement.x == 0 and movement.y == 0) { movement = desired_movement[desired_direction]; }
    if (movement.x != 0)
    { if (desired_movement[desired_direction].x != 0) { movement = desired_movement[desired_direction]; } }
    else if (movement.y != 0)
    { if (desired_movement[desired_direction].y != 0) { movement = desired_movement[desired_direction]; } }

    return;
  }
}

void Player::updateFacing()
{
  if (movement.x == 0 && movement.y == -1) { facing = Facing::NORTH; }
  else if (movement.x == 0 && movement.y == 1) { facing = Facing::SOUTH; }
  else if (movement.x == 1 && movement.y == 0) { facing = Facing::EAST; }
  else if (movement.x == -1 && movement.y == 0) { facing = Facing::WEST; }
}

void Player::keyHandler(sf::Event event)
{
  if (event.type == sf::Event::KeyPressed)
  {
    if (event.key.code == sf::Keyboard::D) { desired_direction = DIRECTIONS::RIGHT; }
    if (event.key.code == sf::Keyboard::A) { desired_direction = DIRECTIONS::LEFT; }
    if (event.key.code == sf::Keyboard::W) { desired_direction = DIRECTIONS::UP; }
    if (event.key.code == sf::Keyboard::S) { desired_direction = DIRECTIONS::DOWN; }
  }
}

void Player::update(float dt)
{
  updateFacing();

  switch (facing)
  {
    case Facing::NONE:
      break;
    case Facing::NORTH:
      sprite_rect = {33, 0, 32, 32};
      sprite->setTextureRect(sprite_rect);
      break;
    case Facing::SOUTH:
      sprite_rect = {65, 0, 32, 32};
      sprite->setTextureRect(sprite_rect);
      break;
    case Facing::EAST:
      sprite_rect = {0, 0, 32, 32};
      sprite->setTextureRect(sprite_rect);
      break;
    case Facing::WEST:
      sprite_rect = {97, 0, 32, 32};
      sprite->setTextureRect(sprite_rect);
      break;
  }

  sprite->move(movement.x * SPEED * dt,
               movement.y * SPEED * dt);

  setMin();
  calculateMax();
}

