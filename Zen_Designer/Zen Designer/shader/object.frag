#version 330 core

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    sampler2D diffTex;
    sampler2D normTex;
    sampler2D specTex;
    vec3 specular;    
    float shininess;
    bool haveDiffTex;
    bool haveNormTex;
    bool haveSpecTex;
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3 TBN;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool haveShadowMap;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    // ambient & diffuse 
    vec3 normal =  vec3(0, 0, 0);
    if (material.haveNormTex) {
        normal = texture(material.normTex, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);   
        normal = normalize(TBN * normal);
    }
    else {
        normal = normalize(Normal);
    }
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = vec3(0, 0, 0);
    vec3 diffuse = vec3(0, 0, 0);
    if (material.haveDiffTex) {
        ambient = light.ambient * texture(material.diffTex, TexCoords).rgb;
        diffuse = light.diffuse * diff * texture(material.diffTex, TexCoords).rgb;
    }
    else {
        ambient = light.ambient * material.ambient;
        diffuse = light.diffuse * (diff * material.diffuse);
    }
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = vec3(0, 0, 0);
    if (material.haveSpecTex) {
        specular = light.specular * spec * texture(material.specTex, TexCoords).rgb;  
    }
    else {
        specular = light.specular * (spec * material.specular);  
    }
        
    vec3 result = vec3(0, 0, 0);
    if (haveShadowMap) {
        float bias = max(0.01 * (1.0 - dot(normalize(Normal), lightDir)), 0.001);
        float shadow = ShadowCalculation(FragPosLightSpace, bias);                      
        result = ambient + (1.0 - shadow) * (diffuse + specular);   
    }
    else {
        result = ambient + diffuse + specular;
    }
    FragColor = vec4(result, 1.0);
}