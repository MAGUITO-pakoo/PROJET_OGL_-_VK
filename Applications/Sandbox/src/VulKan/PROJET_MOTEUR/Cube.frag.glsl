#version 330 core

in vec2 vUV;
in vec3 vTangent;
in vec3 vBitangent;
in vec3 vNormal;
out vec4 FragColor;

uniform sampler2D uDiffuse;
uniform sampler2D uNormalMap;
uniform vec3 uLightDir;

void main() {
    vec3 normal = normalize(vNormal);
    vec3 tangent = normalize(vTangent);
    vec3 bitangent = normalize(vBitangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    vec3 normalSample = texture(uNormalMap, vUV).rgb;
    normalSample = normalize(normalSample * 2.0 - 1.0);
    vec3 worldNormal = normalize(TBN * normalSample);

    vec3 diffuseColor = texture(uDiffuse, vUV).rgb;
    float nDotL = max(dot(worldNormal, normalize(uLightDir)), 0.0);
    vec3 lighting = diffuseColor * (0.3 + 0.7 * nDotL);

    FragColor = vec4(lighting, 1.0);
}
