#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;



uniform mat4 projection;
uniform mat4 transform;
uniform mat4 view;





void main()
{
    
  
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
 
    
    
}