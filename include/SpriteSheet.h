#ifndef _SPRITE_SHEET_H_
#define _SPRITE_SHEET_H_

#include <string>
#include <glm/vec4.hpp>

#include "TextureManager.h"

class SpriteSheet
{
public:
  void initialize(const std::string& fileName, unsigned, unsigned);
  GLuint getNCols();
  GLuint getNRows();
  GLuint getTextureID();
  glm::vec4 getUVs(unsigned index);
  
private:
  unsigned sheetWidth, sheetHeight;
  unsigned spriteWidth, spriteHeight;
  GLuint textureID;

  unsigned nCols, nRows;
};

#endif
