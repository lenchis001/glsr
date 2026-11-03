#ifndef GLSR_TEXTURE_SERVICE_H
#define GLSR_TEXTURE_SERVICE_H

#include <map>
#include <memory>
#include <vector>

#include "ITextureService.h"

#include "Models/TextureModel.h"
#include "Models/Vector4d.h"

using namespace glsr::Services::Texture::Models;

namespace glsr::Services::Texture
{
    class TextureService : public ITextureService
    {
    public:
        TextureService();

        void genTextures(srGLsizei n, srGLuint *textures) override;

        void activeTexture(srGLenum texture) override;

        void bindTexture(srGLenum target, srGLuint texture) override;

        void texParameteri(srGLenum target, srGLenum pname, srGLint param) override;

        void texImage2D(
            srGLenum target,
            srGLint level,
            srGLint internalFormat,
            srGLsizei width,
            srGLsizei height,
            srGLint border,
            srGLenum format,
            srGLenum type,
            const void *data) override;
        
        void texSubImage2D(
            srGLenum target,
            srGLint level,
            srGLint xoffset,
            srGLint yoffset,
            srGLsizei width,
            srGLsizei height,
            srGLenum format,
            srGLenum type,
            const void *data) override;

        void deleteTextures(srGLsizei n, const srGLuint *textures) override;

        TextureModel &getTexture(srGLenum textureUnit, srGLenum target) override;

    private:
        std::map<srGLuint, TextureModel> _textures;

        srGLuint _nextTextureId;
        srGLenum _activeTextureUnit;
        std::map<srGLenum, std::map<srGLenum, srGLuint>> _boundTextures;
    };
} // namespace glsr::Services::Texture

#endif // GLSR_TEXTURE_SERVICE_H