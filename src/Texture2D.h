//
// Created by Benjamin Meyer on 25.12.2020.
//
#pragma once

#include <libheaders.h>
#include <glerror.h>
#include "ShaderProgram.h"
#include <string>

class Texture2D {
public:
    Texture2D(const std::string & path, bool genMipMaps, GLint wrapping = GL_REPEAT, GLint filtering = GL_LINEAR);
    GLuint getTexId() { return texid; };

    inline GLint maxTextureUnits()
    {
        static GLint mtu = -1;
        if (mtu == -1)
        {
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &mtu);
        }
        return mtu;
    }
    static std::string fallbackTexturePath;
private:

    GLuint texid = 0;
};
