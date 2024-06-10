#version 330 core

out vec4 FragColor;

uniform float red;
uniform float green;
uniform float blue;

void main()
{
    
    FragColor = vec4(red,green,blue,0.0f);
     
    
} 