#version 130

in vec2 vertexPosition;
out vec2 fragmentPosition;
uniform mat4 projection;

void main() {
  gl_Position = projection*vec4(vertexPosition, 0.0f, 1.0f);
  fragmentPosition = vertexPosition;  
}
