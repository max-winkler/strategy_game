#include <iostream>
#include <iomanip>

#include <cmath>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Player.h"
#include "Vertex.h"
#include "TextureManager.h"

#define WALK_OFF    0
#define WALK_LEFT   1
#define WALK_RIGHT  2
#define WALK_UP     4
#define WALK_DOWN   8

Player::Player(World* world, float x, float y) :
  Sprite(world, x, y, 0.5f, 0.666f), rotation(0), moveMode(0), prevMoveMode(0), targetPos(0.0f, 0.0f)
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
  
  // Load sprite sheet
  spriteSheet.initialize("images/human.png", 192, 256);

  // Compute initial cell
  currentCell = world->getCellIndices(pos.x, pos.y);
}

void Player::draw()
{
  // Apply camera transformation
  glm::mat4 model = glm::mat4(1.0f);
  glm::vec4 screenPosition = Camera::getCameraMatrix()*glm::vec4(pos.x, pos.y, 0.0f, 1.0f);
  model = glm::translate(model, glm::vec3(screenPosition[0], screenPosition[1], 0.0f));
  model = glm::scale(model, glm::vec3(Camera::getZoom()));
    
  glUniformMatrix4fv(0, 1, GL_FALSE, &(model[0][0]));

  // Set new texture from sprite sheet
  updateAnimation();

  // Bind vertex arrays and textures
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, spriteSheet.getTextureID());

  glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 6 vertices total -> 2 triangle
  glBindVertexArray(0);
  
}

void Player::move(bool se, bool nw, bool ne, bool sw)
{  
  vel.x = ne+se-nw-sw;
  vel.y = ne+nw-se-sw;

  // Normalize
  float norm = sqrt(vel.x*vel.x+vel.y*vel.y);
  if(norm > 0.0f)
    {
      vel.x /= norm;
      vel.y /= norm;
    }

  pos.x += speed*vel.x;
  pos.y += speed*vel.y;
  
  // Determine new walk mode
  int newMoveMode = se*WALK_RIGHT | nw*WALK_LEFT | ne*WALK_UP | sw*WALK_DOWN;
  if(newMoveMode != moveMode)
    {
      prevMoveMode = moveMode;
      moveMode = newMoveMode;
    }
}

void Player::updateAnimation()
{
  unsigned time = SDL_GetTicks();

  int textureIdx = 0;
  bool flip = false;

  if(moveMode == (WALK_RIGHT | WALK_DOWN))
    {
      textureIdx = (time/150)%8+1;
    }
  else if(moveMode == (WALK_LEFT | WALK_UP))
    {
      textureIdx = (time/150)%8+1;
      flip = true;
    }
  else
    {
      // Implement animations for other movement directions when spree sheet is completed
      textureIdx = 0;
      if(prevMoveMode & WALK_LEFT)
        {
	flip = true;
        }
    }
  
  glm::vec4 subtextureUV = spriteSheet.getUVs(textureIdx);
  
  vertices[0].setUV(flip ? subtextureUV.z : subtextureUV.x, subtextureUV.y);
  vertices[1].setUV(flip ? subtextureUV.x : subtextureUV.z, subtextureUV.y);
  vertices[2].setUV(flip ? subtextureUV.z : subtextureUV.x, subtextureUV.w);
  vertices[3].setUV(flip ? subtextureUV.x : subtextureUV.z, subtextureUV.y);
  vertices[4].setUV(flip ? subtextureUV.x : subtextureUV.z, subtextureUV.w);
  vertices[5].setUV(flip ? subtextureUV.z : subtextureUV.x, subtextureUV.w);  
      
  glBindBuffer(GL_ARRAY_BUFFER, vbo);  
  glBufferSubData(GL_ARRAY_BUFFER, 0, 6*sizeof(Vertex), &vertices);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Player::updatePosition()
{
  // Check if movement is necessary
  if(moveMode != 0)
    {
      // Move player

      // Compute velocity
      vel.x = ((moveMode & WALK_RIGHT) != 0) - ((moveMode & WALK_LEFT) != 0);
      vel.y = ((moveMode & WALK_UP) != 0) - ((moveMode & WALK_DOWN) != 0);
      
      // Normalize
      float norm = sqrtf(vel.x*vel.x + vel.y*vel.y);
      if(norm > 0.0f)
        {
	vel.x /= norm;
	vel.y /= norm;
        }
      
      pos.x += speed*vel.x;
      pos.y += speed*vel.y;
      
      // Check if target is reached

      // Get target coordinates
      std::pair<float, float> targetCoords = world->getCellMidpoint(targetCell.first, targetCell.second);
      
      // Check if midpoint of target cell reached
      if(((moveMode & WALK_RIGHT)   != 0 && pos.x > targetCoords.first)
         || ((moveMode & WALK_LEFT) != 0 && pos.x < targetCoords.first)
         || ((moveMode & WALK_UP)   != 0 && pos.y > targetCoords.second)
         || ((moveMode & WALK_DOWN) != 0 && pos.y < targetCoords.second))
        {	    
	pos.x = targetCoords.first;
	pos.y = targetCoords.second;
	
	// correct position
	std::cout << "Reached target " << pos.x << " " << pos.y << std::endl;
	std::cout << "This is cell " << targetCell.first << " " << targetCell.second << std::endl;
	
	// unset move mode
	prevMoveMode = moveMode;
	moveMode = WALK_OFF;
        }
    }
  
  // If player standing still get next part of route from stack
  if(moveMode == WALK_OFF)
    {     
      // Do nothing when no route assigned to player
      if(route.empty())
        return;
	  
      targetCell = route.back();
      route.pop_back();
      currentCell = world->getCellIndices(pos.x, pos.y);

      std::cout << "Pick next part of route.\n";
      std::cout << "Current cell: " << currentCell.first << " " << currentCell.second << std::endl;
      std::cout << "Target cell: " << targetCell.first << " " << targetCell.second << std::endl;
      
      // Calculate movement direction
      int newMoveMode = moveMode;
      if(targetCell.first > currentCell.first)
        moveMode |= WALK_RIGHT;
      else if(targetCell.first < currentCell.first)
        moveMode |= WALK_LEFT;

      if(targetCell.second > currentCell.second)
        moveMode |= WALK_UP;
      else if(targetCell.second < currentCell.second)
        moveMode |= WALK_DOWN;
    }

  
}

void Player::gotoPosition(float targetX, float targetY, std::vector<std::pair<unsigned,unsigned>> route)
{
  targetPos = {targetX, targetY};
  this->route = route;
}
