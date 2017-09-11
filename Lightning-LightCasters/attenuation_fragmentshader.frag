#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shiness;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0/(light.constant + light.linear*distance + light.quadratic*(distance*distance));

    vec3 ambient = vec3(texture(material.diffuse,TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir), 0.0), material.shiness);
//    vec3 specular = light.specular*(spec*vec3(texture(material.specular,TexCoords)));
    vec3 specular = light.specular*(spec*(vec3(1.0f,1.0f,1.0f) - vec3(texture(material.specular,TexCoords) )));

    vec3 result = (ambient + diffuse + specular)*attenuation;

    FragColor = vec4(result, 1.0);
//   FragColor = vec4(0.5,0.5,0.5, 1.0);
}
