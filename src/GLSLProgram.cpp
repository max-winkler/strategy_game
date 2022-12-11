#include "GLSLProgram.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) :
  programId(0)
{
  GLuint vertexShader = loadShader(vertexShaderFile, VERTEX_SHADER);
  GLuint fragmentShader = loadShader(fragmentShaderFile, FRAGMENT_SHADER);
    
  programId = glCreateProgram();
  glAttachShader(programId, vertexShader);
  glAttachShader(programId, fragmentShader);
  
  // Maybe not needed
  // glBindAttribLocation(program[i], 0, "vertexPosition");
  // glBindAttribLocation(program, 1, "vertexColor");

  glLinkProgram(programId);
  
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);  
}

GLSLProgram::~GLSLProgram()
{
}

GLuint GLSLProgram::loadShader(const std::string& fileName, ShaderType shaderType) const
{
  std::ifstream fileStream(fileName.c_str());

  // Get number of characters
  fileStream.seekg(0, std::ios::end);
  int length = fileStream.tellg();

  // Allocate memory and read data
  char* buffer = new char[length+1];
  fileStream.seekg(0);
  fileStream.read(buffer, length);
  buffer[length] = 0;
  
  //printf("Shader source: \n%s\n", buffer);
  
  // Compile shader
  GLuint shader;
  switch(shaderType)
    {
    case ShaderType::VERTEX_SHADER:
      shader = glCreateShader(GL_VERTEX_SHADER);
      break;
    case ShaderType::FRAGMENT_SHADER:
      shader = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    }  
                
  glShaderSource(shader, 1, &buffer, NULL);
  glCompileShader(shader);

  // Error handling
  GLint result = GL_FALSE;
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  
  if ( length > 0 ){
    std::vector<char> errorMessage(length+1);
    glGetShaderInfoLog(shader, length, NULL, &errorMessage[0]);
    printf("Unable to compile shader\n %s\n", &errorMessage[0]);
  }

  delete[] buffer;

  return shader;
}

GLuint GLSLProgram::getId() const
{
  return programId;
}
