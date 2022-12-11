#include "Sprite.h"
#include "Vertex.h"

Sprite::Sprite(float x, float y, float width, float height)
  : pos(x, y), vbo(0), vao(0), alpha(1.0)
{
}

Sprite::~Sprite()
{
}

void Sprite::initialize()
{
  Vertex vertices[6];
  vertices[0].setPosition(-0.5f, -0.5f);
  vertices[1].setPosition(0.5f, -0.5f);
  vertices[2].setPosition(-0.5f, 0.5f);
  vertices[3].setPosition(0.5f, -0.5f);
  vertices[4].setPosition(0.5f, 0.5f);
  vertices[5].setPosition(-0.5f, 0.5f);

  vertices[0].setColor(0.9, 0.6, 0.7, alpha);
  vertices[1].setColor(0.8, 0.7, 0.2, alpha);
  vertices[2].setColor(0.8, 0.7, 0.2, alpha);
  vertices[3].setColor(0.8, 0.7, 0.2, alpha);
  vertices[4].setColor(0.2, 0.4, 0.2, alpha);
  vertices[5].setColor(0.8, 0.7, 0.2, alpha);

  vertices[0].setUV(0.5, 0.0);
  vertices[1].setUV(1.0, 0.5);
  vertices[2].setUV(0.0, 0.5);
  vertices[3].setUV(1.0, 0.5);
  vertices[4].setUV(0.5, 1.0);
  vertices[5].setUV(0.0, 0.5);  
  /*
  vertices[0].setUV(0.0, 0.0);
  vertices[1].setUV(1.0, 0.0);
  vertices[2].setUV(0.0, 1.0);
  vertices[3].setUV(1.0, 0.0);
  vertices[4].setUV(1.0, 1.0);
  vertices[5].setUV(0.0, 1.0);  
  */
  
  glGenVertexArrays(1, &vao);  
  glGenBuffers(1, &vbo);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6*sizeof(Vertex), &(vertices[0].position.x), GL_STATIC_DRAW);
  
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

}

void Sprite::draw()
{
}

void Sprite::setAlpha(float _alpha)
{
  alpha = _alpha;
}
