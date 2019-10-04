#version 430 core

precision mediump float;

const vec3 lightVecNormalized = normalize(vec3(0.0, 5.5, 15));
const vec4 lightColor = vec4(0.2, 0.9, 0.7, 1.0);

in vec3 vNormal;
in vec2 vUV;
in vec4 vColor;

out vec4 fColor;
	
uniform vec3  uAmbientColor;
uniform float uAmbientIntensity;
	
void main()
{
	// Calculate ambient color * intensity
    vec4 ambient = vec4(uAmbientColor, 1.0f) * uAmbientIntensity; 
	
	// workaround for compiler optimizing code
	vec4 tempColorNotUsed = vec4(vNormal.x, vNormal.y, vNormal.z, 1.0);
	vec4 ambColNotUsed = vec4(vUV.x * 0.001, vUV.y * 0.001, 0.001, 0.0);
    	
	// Calculate the light intensity depending on the angle of the light with the fragment
	float diffuseIntensity = clamp(dot(lightVecNormalized, normalize(vNormal)), 0.0, 1.0);
	
	// Calculate the light color at the fragment, multiplying the light color by the intensity at the fragment
	vec4 fLight = lightColor * diffuseIntensity;
	
	// Multiply object color times ambient color
	vec4 objectColor = vColor * ambient;
	
    fColor = normalize(objectColor + fLight);
}
