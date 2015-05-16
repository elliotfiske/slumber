#version 120
attribute vec4 aPosition;
attribute vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying vec3 vNor;

void main() {
    mat4 MV = uViewMatrix * uModelMatrix;
    vec4 pos_cam = MV * aPosition;

	gl_Position = uProjMatrix * pos_cam;

	vNor  = normalize((MV * vec4(aNormal, 0.0)).xyz);
}