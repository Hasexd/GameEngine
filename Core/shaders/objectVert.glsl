#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float outlineWidth = 0.02;
uniform bool isOutlinePass = false;

void main()
{
	vec3 position = aPos;

	if(isOutlinePass)
	{
		position += aNormal * outlineWidth;
	}

	gl_Position = projection * view * model * vec4(position, 1.0);
}