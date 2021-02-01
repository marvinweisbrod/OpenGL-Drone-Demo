//
// Created by Benjamin Meyer on 25.12.2020.
//
#include "Texture2D.h"
#include <iostream>

std::string Texture2D::fallbackTexturePath = std::string("assets/textures/black.png");

Texture2D::Texture2D(const std::string & path, bool genMipMaps, GLint wrapping, GLint filtering)
{
    GLsizei width;
    GLsizei height;
    GLsizei channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (image == nullptr) { // if unsuccessful, try to load the fallback texture
        image = stbi_load(fallbackTexturePath.c_str(), &width, &height, &channels, 0);
        std::cout << "MISSING TEXTURE: " << path << ", loading fallback.\n";
    }
    if (image == nullptr)
        throw std::logic_error("Texture file coudn't be read.");

    else
    {
        GLint internalformat;
        GLenum format;
        switch (channels)
        {
            case 1:
                internalformat = GL_R8;
                format = GL_RED;
                break;
            case 2:
                internalformat = GL_RG8;
                format = GL_RG;
                break;
            case 3:
                internalformat = GL_RGB8;
                format = GL_RGB;
                break;
            case 4:
                internalformat = GL_RGBA8;
                format = GL_RGBA;
                break;
            default:
                internalformat = GL_RGB8;
                format = GL_RGB;
                break;
        }
        glGenTextures(1, &texid); GLERR
        if (texid == 0)
        {
            stbi_image_free(image);
            throw std::logic_error("OpenGL texture object creation failed.");
        }
        glBindTexture(GL_TEXTURE_2D, texid);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            internalformat,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            image
        );
        if (checkglerror())
        {
            glDeleteTextures(1, &texid);
            stbi_image_free(image);
            throw std::logic_error("Error. Could not buffer texture data.");
        }
        if (genMipMaps)
            glGenerateMipmap(GL_TEXTURE_2D); GLERR
            glBindTexture(GL_TEXTURE_2D, 0); GLERR
            stbi_image_free(image);

        glBindTexture(GL_TEXTURE_2D, 0); //unbind
    }
}

Texture2D::Texture2D(const std::vector<std::string>& faces)
{
    // implementation copied from https://learnopengl.com/Advanced-OpenGL/Cubemaps
    stbi_set_flip_vertically_on_load(false);
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texid);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0); //unbind
}

