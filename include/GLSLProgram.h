#ifndef _GLSL_PROGRAM_H_
#define _GLSL_PROGRAM_H_

#include <string>

#include <GL/glew.h>

enum ShaderType {
  VERTEX_SHADER, FRAGMENT_SHADER
};

class GLSLProgram
{
 public:
  GLSLProgram(const std::string&, const std::string&);
  ~GLSLProgram();

  GLuint getId() const;
  
 private:
  GLuint programId;

  GLuint loadShader(const std::string&, ShaderType) const;
};

#endif
