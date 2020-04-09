#version 330 core

// fragment shader

in vec4 fragColor;    // input: interpolated color as rgba-value
in vec2 texCoord;     // input: texture coordinate (xy-coordinates)
out vec4 finalColor;  // output: final color value as rgba-value

uniform sampler2D texture1;

void main() {
//  finalColor = vec4(texCoord,0,0);
  finalColor = texture(texture1, texCoord);
}

