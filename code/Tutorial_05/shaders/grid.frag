#version 330

// grid-line fragmentation shader, darkens grid line fragments that are further away

in vec4 fragColor;
out vec4 fColor;


void main() {
  // Mind: fragDepth may be > 1, that's the reason for the min() call, mix() linearly interpolates
  //       the rgb components
  fColor.rgb = mix(fragColor.rgb, backgroundColor.rgb, min(1, fragDepth));
}

