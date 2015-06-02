uniform mat4 uProjMatrix;
uniform mat4 mirrorViewMat;
uniform mat4 uModelMatrix;
uniform sampler2D reflection_sampler;

varying vec3 vPos;
varying vec2 UV;


void main()
{
   mat4 MVP_Reflect = uProjMatrix * mirrorViewMat * uModelMatrix; 
   vec4 reflectionVertex = MVP_Reflect * vec4(vPos, 1.0);
   
	vec2 vDeviceReflection = reflectionVertex.xy / reflectionVertex.w;
	vec2 vTextureReflection = vec2(0.5, 0.5) + 0.5 * vDeviceReflection;

	vec4 reflectionTextureColor = texture2D (reflection_sampler, UV);

	// Framebuffer reflection can have alpha > 1
	reflectionTextureColor.x = 1.0;

	gl_FragColor = reflectionTextureColor;
}
