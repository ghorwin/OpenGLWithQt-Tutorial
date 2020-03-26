#version 330

// grid-line fragmentation shader, darkens grid line fragments that are further away

in highp vec4 fragColor;
out highp vec4 fColor;

// fragDepth is computed by vertex shader and interpolated
// to the current fragment
in float fragDepth;

uniform vec4 backgroundColor = vec4(0.1, 0.1, 0.2, 1.0);  // background color, that we fade to

void main() {
  // Mind: fragDepth may be > 1, that's the reason for the min() call, mix() linearly interpolates
  //       the rgb components
  fColor.rgb = mix(fragColor.rgb, backgroundColor.rgb, min(1, fragDepth));
}

