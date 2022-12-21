#version 130

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 projection;

void main() {

  gl_Position = projection*vec4(vertexPosition, 0.0f, 1.0f);

  fragmentPosition = vertexPosition;
  fragmentColor = vertexColor;
  fragmentUV = vertexUV;
}
