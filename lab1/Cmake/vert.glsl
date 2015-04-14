attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 uLightPos;
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;

varying vec3 vPos;
varying vec3 vNor;
varying vec3 vLight;
varying vec3 vCol;
varying vec3 vH;
varying vec2 vTexCoord;

void main()
{
	gl_Position = vec4(aPosition, 1);
}
