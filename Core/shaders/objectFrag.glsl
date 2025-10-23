#version 460 core

struct Material
{
    vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

struct Light
{
	vec3 Position;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 Normal;

void main()
{
	vec3 ambient = light.Ambient * material.Ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.Position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.Diffuse) * light.Diffuse;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	vec3 specular = (material.Specular * spec) * light.Specular;

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
}