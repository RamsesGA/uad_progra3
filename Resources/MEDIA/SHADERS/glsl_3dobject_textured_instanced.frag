#version 430 core

precision mediump float;

// Ambient color. Used so the fragment is not pitch black is no light at all touches it
const vec4 ambientLightColor = vec4(0.05, 0.05, 0.05, 1.0);

// Position of the light
const vec3 lightVecNormalized = normalize(vec3(0.0, 5.5, 15));

// Warm yellow-ish light color
const vec4 lightColor = vec4(0.9, 0.87, 0.8, 1.0);

// Varying variables
in vec3 vNormal;
in vec2 vUV;
in vec4 vColor;

// The final fragment color
out vec4 fColor;
	
// A texture sampler
uniform sampler2D textureSampler;
	
void main()
{
	// workaround for compiler optimizing code
	vec4 tempColorNotUsed = vec4(vNormal.x, vNormal.y, vNormal.z, 1.0);
	vec4 ambColNotUsed = vec4(vUV.x * 0.001, vUV.y * 0.001, 0.001, 1.0);
    
	// Sample texture at UV coord for this fragment and multiply it by the object's color
	vec4 texColor = texture2D( textureSampler, vUV ) * vColor;
	
	// Calculate the light intensity depending on the angle of the light with the fragment
	float diffuseIntensity = clamp(dot(lightVecNormalized, normalize(vNormal)), 0.0, 1.0);
	
	// Calculate the light color at the fragment, multiplying the light color by the intensity at the fragment
	vec4 fragmentLight = lightColor * diffuseIntensity;
	
	// Final fragment color is texture color at fragment + light color at fragment + ambient light
	// Final color might go over 1.0, 1.0, 1.0, need to normalize it
    fColor = normalize(texColor + fragmentLight + ambientLightColor);
}
