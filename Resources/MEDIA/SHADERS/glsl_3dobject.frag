#version 430 core

precision mediump float;

const vec4 ambientLight = vec4(0.1, 0.1, 0.1, 1.0);
const vec3 lightVecNormalized = normalize(vec3(0.0, 5.5, 15));
const vec4 lightColor = vec4(0.2, 0.9, 0.7, 1.0);

in vec3 vNormal;
in vec2 vUV;
in vec4 vColor;

out vec4 fColor;
	
//uniform sampler2D textureSampler;
	
void main()
{
    //const vec4 AmbientColor = vec4(0.025, 0.0, 0.075, 1.0);
	
	// workaround for compiler optimizing code
	vec4 tempColor = vec4(vNormal.x, vNormal.y, vNormal.z, 1.0);
	vec4 ambCol = vec4(vUV.x * 0.001, vUV.y * 0.001, 0.001, 1.0);
    //vec4 texColor = texture2D( textureSampler, vUV ) * vColor;
	
	float diffuseIntensity = clamp(dot(lightVecNormalized, normalize(vNormal)), 0.0, 1.0);
	vec4 fAmbient = ambientLight + ambCol;
	vec4 fLight = lightColor * diffuseIntensity;
    fColor = normalize(vColor + fAmbient + fLight);
	
	//fColor = vColor + tempColor + ambCol;
}
