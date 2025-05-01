#version 410 core
out vec4 frag_colour;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct LightStruct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

uniform Material material;
uniform LightStruct light_1;

void main() {
    vec3 ambient = light_1.ambient * material.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_1.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light_1.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light_1.specular * (spec * material.specular);
    
    vec3 result = ambient + diffuse + specular;
    frag_colour = vec4(result, 1.0f);
}