#include <iostream>

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

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 8*numGridLines*sizeof(float), gridLines, GL_STATIC_DRAW);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		    
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  delete[] gridLines;      
}

void World::draw()
{
  glBindVertexArray(vao);
  glDrawArrays(GL_LINES, 0, 8*numGridLines);
  glBindVertexArray(0);
}

void World::getCellCoordinates(unsigned i, unsigned j, float& x, float& y) const  
{
  x = ((float)(i+0.5f)-(float)(numGridLines-1)/2);
  y = ((float)(j+0.5f)-(float)(numGridLines-1)/2);
}

