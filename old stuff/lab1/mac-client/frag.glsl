uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
varying vec3 vH;
varying vec3 vNor;
varying vec3 vLight, vLight2;
varying vec3 vCol;

void main()
{
		vec3 newH = normalize(vH);
		vec3 newNor = normalize(vNor);
		vec3 newLight = normalize(vLight);
        vec3 newLight2 = normalize(vLight2);


		vec3 lDiffuseColor  = vec3(1.0, 1.0, 1.0) * max(0.0, dot(normalize(newNor), normalize(newLight))) * UdColor;
        vec3 lDiffuseColor2 = vec3(1.0, 1.0, 1.0) * max(0.0, dot(normalize(newNor), normalize(newLight2))) * UdColor;
		vec3 lAmbientColor = vec3(1.0, 1.0, 1.0) * UaColor;
		vec3 lSpecularColor = vec3(1.0, 1.0, 1.0) * UsColor * max(0.0, pow(dot(normalize(newNor), normalize(newH)), Ushine));
		gl_FragColor = vec4(lDiffuseColor/2.0 + lDiffuseColor2/2.0 + lAmbientColor + lSpecularColor, 1.0);
	
}
