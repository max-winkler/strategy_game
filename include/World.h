#ifndef _WORLD_H_
#define _WORLD_H_

#include <GL/glew.h>

#include "Vertex.h"
#include "SpriteSheet.h"

enum TileType
  {
    GRASS, WATER, DESSERT
  };

class World
{
 public:
  World(unsigned);
  ~World();

  void initialize(const std::string&);
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

  // Vertices of the two triangles forming a tile
  Vertex* vertices;

  // Map tile types
  TileType* mapTiles;
};

#endif
