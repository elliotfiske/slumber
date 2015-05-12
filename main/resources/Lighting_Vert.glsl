#version 120
attribute vec4 aPosition;
attribute vec3 aNormal;
attribute vec2 aUV;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 lightMVP;
uniform vec3 lightPos;

varying vec3 vPos;
varying vec3 vNor;
varying vec4 shadowClip;
varying vec3 vLight;

varying vec2 UV;

void main() {
    mat4 MV = uViewMatrix * uModelMatrix;
    vec4 pos_cam = MV * aPosition;

    vLight = (MV * vec4(lightPos, 1.0)).xyz;
    vPos = pos_cam.xyz;

	gl_Position = uProjMatrix * pos_cam;

	vNor  = normalize((MV * vec4(aNormal, 0.0)).xyz);

    shadowClip = lightMVP * aPosition;
    
    UV = aUV;
}
