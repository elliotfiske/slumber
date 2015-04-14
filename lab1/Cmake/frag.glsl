uniform sampler2D uTexUnit;
varying vec2 vTexCoord;
uniform int uisTex;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
varying vec3 vH;
varying vec3 vNor;
varying vec3 vLight;
varying vec3 vCol;

void main()
{
	gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
