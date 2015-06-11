#version 120
attribute vec4 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying vec3 vNor;
varying vec3 vPos;


float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    mat4 MV = uViewMatrix * uModelMatrix;
    vec4 pos_cam = MV * aPosition;
    vPos = pos_cam.xyz;
    
    float diffX = rand(pos_cam.xy);
    float diffY = rand(vec2(pos_cam.y, pos_cam.z));
    float diffZ = rand(vec2(pos_cam.z, pos_cam.x));

	gl_Position = uProjMatrix * pos_cam;
	gl_Position += vec4(aNormal, 0.0) * 0.0001; // NOTE: this is a horrible hack so that aNormal is FORCED to be used.  But I don't care.

	gl_Position.x += diffX;
	gl_Position.y += diffY;
	gl_Position.z += diffZ;

	vNor = normalize((MV * vec4(aNormal, 0.0)).xyz);
}
