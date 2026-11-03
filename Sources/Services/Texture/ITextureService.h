#ifndef GLSR_ITEXTURE_SERVICE_H
#define GLSR_ITEXTURE_SERVICE_H

#include <memory>

#include "srtypes.h"

#include "Models/TextureModel.h"

namespace glsr::Services::Texture
{
    class ITextureService
    {
    public:
        virtual ~ITextureService() = default;

        virtual void genTextures(srGLsizei n, srGLuint *textures) = 0;

        virtual void activeTexture(srGLenum texture) = 0;

        virtual void bindTexture(srGLenum target, srGLuint texture) = 0;

        virtual void texParameteri(srGLenum target, srGLenum pname, srGLint param) = 0;

        virtual void texImage2D(
            srGLenum target,
            srGLint level,
            srGLint internalFormat,
            srGLsizei width,
            srGLsizei height,
            srGLint border,
            srGLenum format,
            srGLenum type,
            const void *data) = 0;

        virtual void texSubImage2D(
            srGLenum target,
            srGLint level,
            srGLint xoffset,
            srGLint yoffset,
            srGLsizei width,
            srGLsizei height,
            srGLenum format,
            srGLenum type,
            const void *data) = 0;

        virtual void deleteTextures(srGLsizei n, const srGLuint *textures) = 0;

        virtual Models::TextureModel &getTexture(srGLenum textureUnit, srGLenum target) = 0;
    };
} // namespace glsr::Services::Texture

#endif // GLSR_ITEXTURE_SERVICE_H