#version 430 core

precision mediump float;
in vec4 vColor;
//in vec2 vUV;

out vec4 fColor;
	
//uniform sampler2D tex;	
	
void main()
{
    //vec4 texColor = texture2D( tex, uv ) * vColor;
	fColor = vColor;
}
