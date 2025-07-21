#version 330 core
out vec4 FragColor;

struct VertexAttribute {
    vec3 fragPos;
    vec3 normal;
    vec2 textureCoord;
};
in VertexAttribute vertexAttribute;

struct PointLight {
    vec3 position;

    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight[2];

struct Material {
    vec3 diffuse;
    vec3 specular;
};
uniform Material material;

struct Camera {
    vec3 position;
};
uniform Camera camera;

vec3 calculatePointLight(PointLight light) {
    vec3 color = vec3(0.0f);

    vec3 lightDir = normalize(light.position - vertexAttribute.fragPos);
    float diff = max(0.0f, dot(lightDir, vertexAttribute.normal));
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 viewDir = normalize(camera.position - vertexAttribute.fragPos);
    vec3 reflectDir = reflect(lightDir, vertexAttribute.normal);
    float spec = pow(max(0.0f, dot(-reflectDir, viewDir)), 16.0f);
    vec3 specular = light.specular * spec * material.specular;

    color += diffuse + specular;
    return color;
}

void main()
{
    vec3 color = vec3(0.0f);
    for (int i = 0; i < 1; i++) {
        color += calculatePointLight(pointLight[i]);
    }
    FragColor = vec4(color, 1.0f);
}
