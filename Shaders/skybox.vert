#version 330 core

//get the vertex position of the Skybox
layout(location = 0) in vec3 aPos;

//a cubemap takes in a vec3 instead of 2
out vec3 texCoord;

//creates a projection variable we'll assign the projection matrix here
uniform mat4 projection;

//creates a view variable we'll assign the view matrix here
uniform mat4 view;

void main(){
	//we don't need a tranform matrix for the skybox
	vec4 pos = projection *  //multiply the projection matrix with the view
				view * //multiply the view with the position
				vec4(aPos, 1.0);

	//directly write onto view space
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	
	//assign texcoord with the vertex position directly
	texCoord = aPos;
}