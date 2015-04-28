attribute vec3 aPosition;
attribute vec3 aNormal;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;

varying vec3 vPos;
varying vec3 vNor;
varying vec3 vLight, vLight2;
varying vec3 vCol;
varying vec3 vH;

void main()
{
    mat4 MV = uViewMatrix * uModelMatrix;
	gl_Position = uProjMatrix * MV * vec4(aPosition, 1.0);

	vec3 newNorm;
	vec3 newLight, newLight2;
	vec3 newPos;
	newPos = normalize(vec3(MV * vec4(aPosition, 0.0)));
	newNorm = normalize(vec3(MV * vec4(aNormal, 0.0)));
	newLight =  normalize(vec3(uViewMatrix * vec4(-0.2, -0.7, 2.0, 0.0)));
    newLight2 = normalize(vec3(uViewMatrix * vec4( 0.2, -0.7, 2.0, 0.0)));
	
	vec3 V = normalize(vec3(0.0, 0.0, 0.0) - newPos);
	vec3 H = normalize(newLight + V);

	vNor = newNorm;
    vLight = newLight;
    vLight2 = newLight2;
	vH = H;
}
