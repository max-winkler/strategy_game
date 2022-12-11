#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <map>
#include <string>
#include <GL/glew.h>

class TextureManager
{
 public:
  static void loadTexture(const std::string& fileName, unsigned char*& data,
		      unsigned& width, unsigned& height);

  static GLuint getTextureID(const std::string&);
  static void addTexture(const std::string&, GLuint);
  

  static std::map<std::string, GLuint> textures;
 private:
};

#endif
