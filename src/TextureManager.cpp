#include <png.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "TextureManager.h"

std::map<std::string, GLuint> TextureManager::textures;

void TextureManager::loadTexture(const std::string& fileName, unsigned char*& data,
			   unsigned& width, unsigned& height)
{
  FILE *fp = fopen(fileName.c_str(), "r");
  if(!fp)
    {
      std::cout << "Can not open texture file " << fileName << std::endl;
    }
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);

  if(!png)
    std::cout << "Cant read texture" << std::endl;
  if(!info)
    std::cout << "Cant read texture info" << std::endl;

  png_init_io(png, fp);
  png_read_info(png, info);
  
  width = png_get_image_width(png, info);
  height = png_get_image_height(png, info);

  std::cout << "Image: " << width << "x" << height << std::endl;
  
  int color_type = png_get_color_type(png, info);
  switch(color_type)
    {
    case PNG_COLOR_TYPE_PALETTE:
      std::cout << "palette" << std::endl;
      break;
    case PNG_COLOR_TYPE_GRAY:
      std::cout << "Grau" << std::endl;
      break;
    case PNG_COLOR_TYPE_RGB:
      std::cout << "RGB" << std::endl;
      break;      
    case PNG_COLOR_TYPE_RGBA:
      std::cout << "RGBA" << std::endl;
      break;
    }

  data = new unsigned char[4*height*width];
    
  unsigned char** row_pointers = new unsigned char*[height];
  for(int i=0; i<height; ++i)
    row_pointers[i] = &(data[4*i*width]);
  
  png_read_image(png, row_pointers);
}

GLuint TextureManager::getTextureID(const std::string& fileName)
{
  auto it = textures.find(fileName);
  if(it == textures.end())
    {
      GLuint textureID;
      unsigned char* textureData;
      unsigned textureWidth, textureHeight;
      TextureManager::loadTexture(fileName.c_str(), textureData, textureWidth, textureHeight);
      
      glGenTextures(1, &textureID);
      glBindTexture(GL_TEXTURE_2D, textureID);
      std::cout << "Created texture with ID " << textureID << std::endl;
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight,
	         0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
      glGenerateMipmap(GL_TEXTURE_2D);
      
      glBindTexture(GL_TEXTURE_2D, 0);

      TextureManager::addTexture(fileName, textureID);

      // Delete texture data
      delete[] textureData;  
      return textureID;
    }

  return it->second;
}

void TextureManager::addTexture(const std::string& fileName, GLuint textureID)
{
  textures[fileName] = textureID;
}
