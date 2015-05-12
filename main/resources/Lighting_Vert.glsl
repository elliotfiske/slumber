#version 120
attribute vec3 aPosition;
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

    vLight = vec3(MV * vec4(lightPos, 1.0));
    vPos = vec3(MV * vec4(aPosition, 1.0));

	gl_Position = uProjMatrix * MV * vec4(aPosition, 1.0);

	vNor  = normalize(vec3(MV * vec4(aNormal, 0.0)));

    shadowClip = lightMVP * vec4(aPosition, 1.0);
    
    UV = aUV;
}
