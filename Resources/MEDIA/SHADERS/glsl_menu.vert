#version 430 core

layout( location = 0 ) in vec3 attPosition;
layout( location = 1 ) in vec2 attUV;

uniform vec3 uColor;

out vec4 vColor;
out vec2 vUV;

void main()
{
    vec4 aPos4 = vec4(attPosition, 1.0f);
    vec4 aCol4 = vec4(uColor, 1.0f);
	
	gl_Position = aPos4;

	vColor = aCol4;
	vUV = attUV;
}
