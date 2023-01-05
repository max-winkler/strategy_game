#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>

#include "Sprite.h"
#include "SpriteSheet.h"

class Player : public Sprite
{
 public:
  Player(World*, float, float);
  ~Player();

  void initialize();
  void updatePosition();
  void draw();
  void updateAnimation();
  void gotoPosition(float, float, std::vector<std::pair<unsigned,unsigned>>);
  void move(bool nw, bool se, bool ne, bool sw);
  
  struct Velocity{
    float x = 0;
    float y = 0;
  } vel;

  float rotation;
  
 private:
  int moveMode, prevMoveMode;

  // Store target position and current tile path to that position
  Position targetPos;
  std::vector<std::pair<unsigned,unsigned>> route;
  std::pair<unsigned,unsigned> currentCell;
  std::pair<unsigned,unsigned> targetCell;

  // Player properties
  const float speed = 0.01f;
};

#endif
