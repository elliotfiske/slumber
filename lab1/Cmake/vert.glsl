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
    mat4 MV = uViewMatrix * uModelMatrix;
	gl_Position = uProjMatrix * MV * vec4(aPosition, 1.0f);

	vec3 newNorm;
	vec3 newLight;
	vec3 newPos;
	newPos = normalize(vec3(MV * vec4(aPosition, 0.0f)));
	newNorm = normalize(vec3(MV * vec4(aNormal, 0.0f)));
	newLight = normalize(vec3(uViewMatrix * vec4(uLightPos, 0.0f)));
	
	vec3 V = normalize(vec3(0.0, 0.0, 0.0) - newPos);
	vec3 H = normalize(newLight + V);

	vNor = newNorm;
	vLight = newLight;
	vH = H;
	vTexCoord = aTexCoord;
}