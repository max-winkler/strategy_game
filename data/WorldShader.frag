#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 frag_color;

uniform sampler2D tileTexture;

void main() {
  vec4 tex_color = texture(tileTexture, fragmentUV);
  frag_color = vec4(tex_color.rgb, tex_color.a*fragmentColor.a);
  //frag_color = vec4(0.1, 0.2, 0.3, 1.0);
}
