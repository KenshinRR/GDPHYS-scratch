#version 330 core

uniform sampler2D tex0;

uniform vec3 lightPos;

uniform vec3 lightColor;

uniform float ambientStr;

uniform vec3 ambientColor;

uniform vec3 cameraPos;

uniform float specStr;

uniform float specPhong;

uniform vec3 direction;

uniform float brightness;

uniform float dl_brightness;

out vec4 FragColor;

in vec2 texCoord;

in vec3 normCoord;

in vec3 fragPos;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(vec3 normal, vec3 viewDir);

void main()
{
	vec3 result;

	vec3 norm = normalize(normCoord);
	vec3 viewDir = normalize(cameraPos - fragPos);

	//directional light
	result = CalcDirLight(norm, viewDir);
	
	//point light
	result += CalcPointLight(norm, viewDir);

	//multiply the attenuation to the sum of the diffuse, ambientCol, and specColor to implement it
	//FragColor = vec4(diffuse + ambientCol + specColor,1.0) * texture(tex0, texCoord);
	FragColor = vec4(result, 1.0)  * texture(tex0, texCoord);;
}


vec3 CalcDirLight(vec3 normal, vec3 viewDir){

	vec3 lightDir = normalize(-direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);

	vec3 ambient = ambientColor * ambientStr;
	vec3 diffuse = diff * lightColor;
	vec3 specular = spec * specStr * lightColor;

	return (ambient + diffuse + specular) * dl_brightness;
}

vec3 CalcPointLight(vec3 normal, vec3 viewDir){

	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);

	//POINT LIGHT
	//compute for the attenuation which is the change in light intensity over the change in distance
	float flDistance = length(lightPos - fragPos); //get the distance between the light and the fragment object
	float linear = 0.09f, quadratic = 0.032f; //constants for the attenuation formula
	float attenuation = brightness /(quadratic * flDistance * flDistance + linear * flDistance + 1.0f); //computing the actual intensity or attenuation

	vec3 ambient = ambientColor * ambientStr;
	vec3 diffuse = diff * lightColor;
	vec3 specular = spec * specStr * lightColor;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);

}