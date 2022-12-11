#include "Vertex.h"

void Vertex::setPosition(float x, float y)
{
  position.x = x;
  position.y = y;
}

void Vertex::setColor(float r, float g, float b, float a)
{
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;
}

void Vertex::setUV(float u, float v)
{
  uv.u = u;
  uv.v = v;
}
