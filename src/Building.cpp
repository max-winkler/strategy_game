#include <iostream>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Building.h"
#include "Vertex.h"
#include "TextureManager.h"

Building::Building(float x, float y) :
  Sprite(x, y, 1.0f, 1.0f)
{
}

Building::~Building()
{
}

void Building::initialize()
{
    Sprite::initialize();
  
  // Load texture  
  textureID = TextureManager::getTextureInfo("building_1.png").textureID;
}

void Building::draw()
{  
  // Update Position
  glm::mat4 model = glm::mat4(1.0f);
  glm::vec4 screenPosition = Camera::getCameraMatrix()*glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
  model = glm::translate(model, glm::vec3(screenPosition[0], screenPosition[1], 0.0f));
  model = glm::scale(model, glm::vec3(Camera::getZoom()));
  
  glUniformMatrix4fv(0, 1, GL_FALSE, &(model[0][0]));
  
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 6 vertices total -> 2 triangle
  glBindVertexArray(0);
}

void Building::setPosition(float x, float y)
{
  pos.x = x;
  pos.y = y;
}
