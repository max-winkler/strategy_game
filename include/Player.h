#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Sprite.h"

class Player : public Sprite
{
 public:
  Player(float, float);
  ~Player();

  void initialize();
  void draw();

  struct Velocity{
    int x = 0;
    int y = 0;
  } vel;

  float rotation;
  
 private:
  
  const float size = 1.0;
};

#endif
