#version 330 core

// fragment shader

in vec2 texCoord;     // input: texture coordinate (xy-coordinates within the texture)
//flat in float texID;  // input: textureID
out vec4 finalColor;  // output: final color value as rgba-value

uniform sampler2D text01; // the font texture

void main() {
  // todo switch texture based on texID
  finalColor = texture(text01, texCoord);
}

