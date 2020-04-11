#version 330 core

// fragment shader

in vec4 fragColor;    // input: interpolated color as rgba-value
in vec2 texCoord;     // input: texture coordinate (xy-coordinates)
flat in float texID;    // input: textureID
out vec4 finalColor;  // output: final color value as rgba-value

uniform sampler2D brickTexture; // the wall texture
uniform sampler2D plasterTexture; // the plaster texture
uniform sampler2D rooftiles; // the roof texture

void main() {
//  finalColor = vec4(texCoord,0,0);
  if (texID == 0)
    finalColor = texture(brickTexture, texCoord);
  else if (texID == 2)
    finalColor = texture(rooftiles, texCoord);
  else if (texID == 1)
    finalColor = texture(plasterTexture, texCoord);
  else
    finalColor = vec4(texCoord,0,0);//texture(rooftiles, texCoord);
}

