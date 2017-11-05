#version 400 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D Textura;

void main()
{

	color=vec4(vec3(texture(Textura, texCoord)),1.0f);
}