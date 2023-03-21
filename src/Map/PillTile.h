//
// Created by perwi on 18/11/2022.
//

#ifndef PACMANSFML_PILLTILE_H
#define PACMANSFML_PILLTILE_H

#include "Tile.h"

class PillTile : public Tile
{
 public:
  PillTile(sf::Texture &_texture, int x, int y, int texture_num, int _cost);
  ~PillTile() = default;

  int getValue() const;
  void setValue(int _value);

  bool getPower() const;
  void setPower(bool _power);

 private:
  int value;
  bool power;
};

#endif // PACMANSFML_PILLTILE_H
