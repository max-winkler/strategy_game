#include <iostream>
#include <random>

#include <glm/gtc/matrix_transform.hpp>

#include "World.h"

World::World(unsigned x) :
  worldSize(2*x*cellWidth),
  numGridLines(2*x+1)
{
}

World::~World()
{
}

void World::initialize()
{
  spriteSheet.initialize("images/tilesheet.png", 108, 108);
  numGridLines = worldSize/cellWidth+1;

  // Initialize random number generator
  std::mt19937 gen;
  std::binomial_distribution<int> idist(15, 0.2);
  
  vertices = new Vertex[2*3*numGridLines*numGridLines];
  for(int i=0; i<numGridLines; ++i)
    {
      float x = (float)i-(float)(numGridLines-1)/2.0f;
      for(int j=0; j<numGridLines; ++j)
        {
	float y = (float)j-(float)(numGridLines-1)/2.0f;
	
	// Set vertex positions in sprite
	vertices[6*(numGridLines*i+j) + 0].setPosition(x+0.0f, y+0.0f);
	vertices[6*(numGridLines*i+j) + 1].setPosition(x+1.0f, y+0.0f);
	vertices[6*(numGridLines*i+j) + 2].setPosition(x+1.0f, y+1.0f);
	vertices[6*(numGridLines*i+j) + 3].setPosition(x+0.0f, y+0.0f);
	vertices[6*(numGridLines*i+j) + 4].setPosition(x+1.0f, y+1.0f);
	vertices[6*(numGridLines*i+j) + 5].setPosition(x+0.0f, y+1.0f);
	
	// Set UV coordinates in sprite
	int textureIdx = idist(gen)%15;
	glm::vec4 uv = spriteSheet.getUVs(textureIdx);
	
	vertices[6*(numGridLines*i+j) + 0].setUV(0.5*(uv.x+uv.z), uv.w);
	vertices[6*(numGridLines*i+j) + 1].setUV(uv.z, 0.25*uv.y + 0.75*uv.w);
	vertices[6*(numGridLines*i+j) + 2].setUV(0.5*(uv.x+uv.z), 0.5*uv.y + 0.5*uv.w);
	vertices[6*(numGridLines*i+j) + 3].setUV(0.5*(uv.x+uv.z), uv.w);
	vertices[6*(numGridLines*i+j) + 4].setUV(0.5*(uv.x+uv.z), 0.5*uv.y + 0.5*uv.w);
	vertices[6*(numGridLines*i+j) + 5].setUV(uv.x, 0.25*uv.y + 0.75*uv.w);
        }
    }
  
  // OLD VERSION
  /*
    numGridLines = worldSize/cellWidth+1;

  float* gridLines = new float[8*(numGridLines)];
  for(unsigned i=0; i<numGridLines; ++i)
    {
      gridLines[4*i] = ((float)i-(float)(numGridLines-1)/2);
      gridLines[4*i+1] = -(float)(numGridLines-1)/2;
      gridLines[4*i+2] = ((float)i-(float)(numGridLines-1)/2);
      gridLines[4*i+3] = (float)(numGridLines-1)/2;
    }
  for(unsigned i=0; i<numGridLines; ++i)
    {
      gridLines[4*numGridLines+4*i] = -(float)(numGridLines-1)/2;
      gridLines[4*numGridLines+4*i+1] = ((float)i-(float)(numGridLines-1)/2);
      gridLines[4*numGridLines+4*i+2] = (float)(numGridLines-1)/2;
      gridLines[4*numGridLines+4*i+3] = ((float)i-(float)(numGridLines-1)/2);
    }
  */
  
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6*numGridLines*numGridLines*sizeof(Vertex), &(vertices[0].position.x), GL_DYNAMIC_DRAW);
  
  // glBufferData(GL_ARRAY_BUFFER, 8*numGridLines*sizeof(float), gridLines, GL_STATIC_DRAW);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));

  //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  		    
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  // delete[] gridLines;      
}

void World::draw()
{
  glBindVertexArray(vao);
  // glDrawArrays(GL_LINES, 0, 8*numGridLines);

  glBindTexture(GL_TEXTURE_2D, spriteSheet.getTextureID());
  glDrawArrays(GL_TRIANGLES, 0, 6*numGridLines*numGridLines); // Starting from vertex 0; 6 vertices total -> 2 triangle  
  glBindVertexArray(0);
}

void World::getCellCoordinates(unsigned i, unsigned j, float& x, float& y) const  
{
  x = ((float)(i+0.5f)-(float)(numGridLines-1)/2);
  y = ((float)(j+0.5f)-(float)(numGridLines-1)/2);
}
