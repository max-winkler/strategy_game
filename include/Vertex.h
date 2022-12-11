#ifndef _VERTEX_H_
#define _VERTEX_H_

struct Vertex {
  struct Position {
  Position(float x, float y) : x(x), y(y) {}
    float x;
    float y;
  } position;
  struct Color {
    float r;
    float g;
    float b;
    float a;
  } color;
  struct UV {
    float u;
    float v;
  } uv;
  
Vertex() : position(0.0f, 0.0f) {}
Vertex(float x, float y) : position(x,y) {}

  void setPosition(float, float);
  void setColor(float, float, float, float);
  void setUV(float, float);
};

#endif
