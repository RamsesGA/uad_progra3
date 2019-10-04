#version 430 core

precision mediump float;

const vec4 ambient = vec4(0.5, 0.5, 0.5, 1.0);

in vec4 vColor;

out vec4 fColor;	
	
void main()
{
    fColor = normalize(vColor + ambient);	
}
