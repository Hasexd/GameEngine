#version 460 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 outlineColor = vec3(1.0, 0.5, 0.0);
uniform bool isOutlinePass = false;

void main()
{

	if(isOutlinePass)
	{
		FragColor = vec4(outlineColor, 1.0);
	}
	else 
	{
		FragColor = vec4(objectColor * lightColor, 1.0);
	}
}