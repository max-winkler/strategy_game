#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "Sprite.h"

class Building : public Sprite
{
 public:
  Building(World* world, float, float);
  ~Building();

  void initialize();
  void draw();
  void setPosition(float, float);
};

#endif
