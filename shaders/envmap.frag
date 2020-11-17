#version 330 core

in vec3 worldPos;
in vec3 worldNormal;

uniform vec3 eyePos;
uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
    // viewing direction
    vec3 eyeVec = normalize(eyePos - worldPos);
    // reflected vector
    vec3 reflectVec = -reflect(eyeVec, worldNormal);

    // get texture value and output frag color
    FragColor = vec4(texture(skybox, reflectVec).rgb, 1.0);
}
