#version 330 core

out vec4 color_out;

in vec3 textureDir; // direction vector representing a 3D texture coordinate
uniform samplerCube cubemap; // cubemap texture sampler

void main()
{             
    color_out = texture(cubemap, textureDir);
}  