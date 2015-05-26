#version 120
attribute vec4 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying vec3 vNor;
varying vec3 vPos;

void main() {
    mat4 MV = uViewMatrix * uModelMatrix;
    vec4 pos_cam = MV * aPosition;
    vPos = pos_cam.xyz;

	gl_Position = uProjMatrix * pos_cam;
	gl_Position += vec4(aNormal, 0.0) * 0.0001; // NOTE: this is a horrible hack so that aNormal is FORCED to be used.  But I don't care.
	

	vNor = normalize((MV * vec4(aNormal, 0.0)).xyz);
}