#version 120

attribute vec4 vertPos;

uniform vec4 vertColor;
uniform mat4 MVP;

varying vec4 color;

void main()
{
	color = vertColor;
	gl_Position = MVP * vertPos;
}
