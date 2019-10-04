#version 430 core

layout( location = 0 ) in vec3 attPosition;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform vec3 uColor;

out vec4 vColor;

void main()
{
    vec4 aPos4 = vec4(attPosition, 1.0f);
    vec4 aCol4 = vec4(uColor, 1.0f);
	
	gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPos4;

	vColor = aCol4;
}
