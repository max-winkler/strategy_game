#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Sprite.h"
#include "SpriteSheet.h"

class Player : public Sprite
{
 public:
  Player(float, float);
  ~Player();

  void initialize();
  void draw();
  void updateAnimation();
  
  void move(bool nw, bool se, bool ne, bool sw);
  
  struct Velocity{
    float x = 0;
    float y = 0;
  } vel;

  float rotation;
  
 private:
  int moveMode, prevMoveMode;
};

#endif
