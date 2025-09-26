#include <iostream>

#include "SpriteSheet.h"

void SpriteSheet::initialize(const std::string& fileName, unsigned spriteWidth, unsigned spriteHeight)
  
{
  this->spriteWidth= spriteWidth;
  this->spriteHeight = spriteHeight;
    
  TextureInfo textureInfo = TextureManager::getTextureInfo(fileName);

  textureID = textureInfo.textureID;
  sheetWidth = textureInfo.width;
  sheetHeight = textureInfo.height;

  nCols = sheetWidth / spriteWidth;
  nRows = sheetHeight / spriteHeight;

  std::cout << "Loaded sprite sheet of size " << sheetWidth << "x" << sheetHeight
	  << " with tiles of size " << spriteWidth << "x" << spriteHeight << std::endl;
}

GLuint SpriteSheet::getNCols()
{
  return nCols;
}

GLuint SpriteSheet::getNRows()
{
  return nRows;
}

GLuint SpriteSheet::getTextureID()
{
  return textureID;
}

glm::vec4 SpriteSheet::getUVs(unsigned index)
{
  glm::vec4 uvs;

  uvs.x = ((index % nCols) * (float)spriteWidth) / sheetWidth;
  uvs.y = ((index / nCols) * (float)spriteHeight) / sheetHeight;
  uvs.z = ((index % nCols + 1) * (float)spriteWidth - 1) / sheetWidth;
  uvs.w = ((index / nCols + 1) * (float)spriteHeight - 1) / sheetHeight;

  return uvs;
}
