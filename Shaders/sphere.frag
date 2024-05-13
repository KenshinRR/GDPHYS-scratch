#version 330 core

//		RGB values
uniform float red;
uniform float green;
uniform float blue;

//		EXPORT
out vec4 FragColor;

void main()
{
	//FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	FragColor = vec4(red, green, blue, 1.0f);
}
