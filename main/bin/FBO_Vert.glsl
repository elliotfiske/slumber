#version 120

// Position - in this case, of the 2 triangles that cover the whole screen.
attribute vec3 aPosition;

// Interpolate the vertex positions and pass them to the fragment shader.
varying vec2 UV;

void main() {
	gl_Position = vec4(aPosition, 1);
	UV = (aPosition.xy + vec2(1,1)) / 2.0;
}