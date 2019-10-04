#version 430 core

layout( location = 0 ) in vec3 attPosition;
layout( location = 1 ) in vec3 attNormal;
layout( location = 2 ) in vec2 attUV;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform vec3 uColor;

out vec3 vNormal;
out vec2 vUV;
out vec4 vColor;

void main()
{
    vec4 aPos4 = vec4(attPosition, 1.0f);
    vec4 aCol4 = vec4(uColor, 1.0f);
	
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPos4;

	vNormal = normalize((uViewMatrix * uModelMatrix * vec4(attNormal, 0)).xyz);
	vUV = attUV;
	vColor = aCol4;
}
