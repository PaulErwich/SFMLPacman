//
// Created by perwi on 18/11/2022.
//

#include "PillTile.h"
PillTile::PillTile(
  sf::Texture& _texture, int x, int y, int texture_num, int _cost) :
  Tile(_texture, x, y, texture_num, _cost)
{
  if (texture_num == 64)
  {
    value = 10;
    power = false;
  }
  else if (texture_num == 65)
  {
    value = 100;
    power = true;
  }
  else
  {
    value = 0;
    power = false;
  }
}

int PillTile::getValue() const { return value; }
void PillTile::setValue(int _value) { value = _value; }

bool PillTile::getPower() const { return power; }
void PillTile::setPower(bool _power) { power = _power; }
