attribute vec4 vertPos;
attribute vec3 aNormal;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

varying vec3 vPos;


void main()
{
   mat4 MVP = uProjMatrix * uViewMatrix * uModelMatrix;
  
	gl_Position = MVP * vertPos;
	
	vPos = vertPos.xyz;

}
