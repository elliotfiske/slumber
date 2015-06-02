attribute vec4 vertPos;
attribute vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying vec3 vPos;

// Interpolate the vertex positions and pass them to the fragment shader.
varying vec2 UV;

void main()
{
   mat4 MVP = uProjMatrix * uViewMatrix * uModelMatrix;
  
	gl_Position = MVP * vertPos;
	
	vPos = vertPos.xyz;

    UV = (gl_Position.xy + vec2(1, 1)) /1.0;
}
