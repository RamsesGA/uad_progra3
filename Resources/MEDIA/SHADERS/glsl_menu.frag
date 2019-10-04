#version 430 core

precision mediump float;

in vec4 vColor;
in vec2 vUV;

out vec4 fColor;
	
uniform sampler2D textureSampler;
	
void main()
{
    vec3 texColor = texture2D(textureSampler, vUV).rgb;
	fColor = vColor * vec4(texColor, 1.0);
}
