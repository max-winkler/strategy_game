#ifndef _WORLD_H_
#define _WORLD_H_

#include <GL/glew.h>

#include "Vertex.h"
#include "SpriteSheet.h"

class World
{
 public:
  World(unsigned);
  ~World();

  void initialize();
  void draw();

  void getCellCoordinates(unsigned, unsigned, float&, float&) const;
 private:
  
  const unsigned cellWidth = 128;
  unsigned worldSize;
  unsigned numGridLines;
  GLuint vao;
  GLuint vbo;

  // Sprite sheet containing tiles of the world
  SpriteSheet spriteSheet;

  // 6 vertices of the two triangles forming a tile
  Vertex* vertices;
};

#endif
