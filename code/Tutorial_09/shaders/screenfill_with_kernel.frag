#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float screenHeight = 1360;
uniform float screenWidth = 2505;

void main() {
  float x_offset;
  float y_offset;
  x_offset = 1.0 / screenWidth;
  y_offset = 1.0 / screenHeight;
  vec2 offsets[9] = vec2[](
    vec2(-x_offset,  y_offset), // top-left
    vec2( 0.0f,    y_offset), // top-center
    vec2( x_offset,  y_offset), // top-right
    vec2(-x_offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( x_offset,  0.0f),   // center-right
    vec2(-x_offset, -y_offset), // bottom-left
    vec2( 0.0f,   -y_offset), // bottom-center
    vec2( x_offset, -y_offset)  // bottom-right
  );

  // sharpen kernel
  float kernel[9] = float[] (
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
  );

//  // blur kernel - simulates antialiasing
//  float kernel[9] = float[](
//      1.0 / 16, 2.0 / 16, 1.0 / 16,
//      2.0 / 16, 4.0 / 16, 2.0 / 16,
//      1.0 / 16, 2.0 / 16, 1.0 / 16
//  );

  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
  {
    sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
  }
  vec3 col = vec3(0.0);
  for(int i = 0; i < 9; i++)
    col += sampleTex[i] * kernel[i];

  float average = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
  FragColor = vec4(average, average, average, 1.0);
//  FragColor = vec4(col*1.2, 1.0);
}

