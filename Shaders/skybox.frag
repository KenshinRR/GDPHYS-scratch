#version 330 core

//add in our cubemap for later
uniform samplerCube skybox;

//returns a color
out vec4 FragColor;

//take in vec3 texCoord
in vec3 texCoord;

//simple shader that colors the model red
void main()
{	
	//call the texture function to get the color
	FragColor = texture(skybox, texCoord);
}
