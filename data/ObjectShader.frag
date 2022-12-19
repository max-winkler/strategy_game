#version 130

in vec4 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;
out vec4 frag_color;
uniform sampler2D playerTexture;
uniform vec2 mouse;

void main() {
  vec4 tex_color = texture(playerTexture, fragmentUV);
  frag_color = vec4(tex_color.rgb, tex_color.a*fragmentColor.a);
  //frag_color = fragmentPosition*vec4(1.0, 1.0, 1.0, 1.0);
}
