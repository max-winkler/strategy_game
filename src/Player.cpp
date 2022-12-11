#include <iostream>
#include <iomanip>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Player.h"
#include "Vertex.h"
#include "TextureManager.h"

Player::Player(float x, float y) :
  Sprite(x, y, 0.3, 0.3), rotation(0), size(0.5)
{
}

Player::~Player()
{
  if(vbo > 0)
    glDeleteBuffers(1, &vbo);
}

void Player::initialize()
{
  Sprite::initialize();
  
  // Load texture  
  textureID = TextureManager::getTextureID("howl2.png");
}

void Player::draw()
{  
  // Draw the triangle !
  // Update Position
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
  model = glm::scale(model, glm::vec3(size));
  
  glUniformMatrix4fv(0, 1, GL_FALSE, &(model[0][0]));
  
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 6 vertices total -> 2 triangle
  glBindVertexArray(0);
}
