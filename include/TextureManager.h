#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <map>
#include <string>
#include <GL/glew.h>
#include <glm/vec2.hpp>

struct TextureInfo
{
  GLuint textureID;
  unsigned width, height;
};

class TextureManager
{
 public:
  static void loadTexture(const std::string& fileName, unsigned char*& data,
		      unsigned& width, unsigned& height);

  static TextureInfo getTextureInfo(const std::string&);

  static std::map<std::string, TextureInfo> textures;
 private:
};

#endif
