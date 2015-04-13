uniform sampler2D uTexUnit;
varying vec2 vTexCoord;
uniform int uisTex;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
varying vec3 vH;
varying vec3 vNor;
varying vec3 vLight;
varying vec3 vCol;

void main()
{
	if(uisTex == 1){
		vec4 texColor1 = texture2D(uTexUnit, vTexCoord);
		gl_FragColor = texColor1;
	}
	else {
		vec3 newH = normalize(vH);
		vec3 newNor = normalize(vNor);
		vec3 newLight = normalize(vLight);


		vec3 lDiffuseColor = vec3(1.0, 1.0, 1.0) * max(0.0, dot(normalize(newNor), normalize(newLight))) * UdColor;
		vec3 lAmbientColor = vec3(1.0, 1.0, 1.0) * UaColor;
		vec3 lSpecularColor = vec3(1.0, 1.0, 1.0) * UsColor * max(0.0, pow(dot(normalize(newNor), normalize(newH)), Ushine));
		gl_FragColor = vec4(lDiffuseColor + lAmbientColor + lSpecularColor, 1.0);
	}
}