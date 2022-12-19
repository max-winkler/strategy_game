#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <GL/glew.h>

#include "Vertex.h"
#include "Camera.h"
#include "SpriteSheet.h"

struct Position {
  Position(float x, float y) : x(x), y(y) {}
    float x = 0.0;
    float y = 0.0;
};
  
class Sprite
{
 public:
  Sprite(float, float, float, float);
  ~Sprite();

  void initialize();
  void setAlpha(float);
  void draw();

  Position pos;
  Position size;
  
 protected:
  GLuint vao;
  GLuint vbo;
  float alpha;
  GLuint textureID; // Remove this and use spriteSheet later
  SpriteSheet spriteSheet;
  Vertex vertices[6];
};

#endif
