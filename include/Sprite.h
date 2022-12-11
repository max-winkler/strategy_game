#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <GL/glew.h>

class Sprite
{
 public:
  Sprite(float, float, float, float);
  ~Sprite();

  void initialize();
  void setAlpha(float);
  void draw();

  struct Position {
  Position(float x, float y) : x(x), y(y) {}
    float x = 0.0;
    float y = 0.0;
  } pos;

 protected:
  GLuint vao;
  GLuint vbo;
  GLuint textureID;
  float alpha;
};

#endif
