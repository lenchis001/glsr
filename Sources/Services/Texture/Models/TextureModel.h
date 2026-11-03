#ifndef GLSR_TEXTURE_MODEL_H
#define GLSR_TEXTURE_MODEL_H

#include <cassert>
#include <cstdint>
#include <map>
#include <vector>

#include "srtypes.h"

#include "Models/Vector4d.h"

namespace glsr::Services::Texture::Models
{
    class TextureModel
    {
    public:
        TextureModel();

        void patchData(
            srGLenum target,
            srGLint level,
            srGLint xoffset,
            srGLint yoffset,
            srGLsizei width,
            srGLsizei height,
            srGLenum format,
            srGLenum type,
            const void *data);

        srGLint getLevel() const;
        srGLint getInternalFormat() const;
        srGLsizei getWidth() const;
        srGLsizei getHeight() const;
        srGLint getBorder() const;
        srGLenum getFormat() const;
        srGLenum getType() const;
        const std::vector<glsr::Models::Vector4d<srGLfloat>> &getPixelData() const;

        srGLint getParameter(srGLenum key, srGLint defaultValue) const;

        void setParameter(srGLenum key, srGLint value);

    private:
        std::vector<glsr::Models::Vector4d<srGLfloat>> _decodePixels(
            const uint8_t *bytes,
            srGLsizei pixelWidth = -1,
            srGLsizei pixelHeight = -1) const;

        srGLint _levelOfDetail;
        srGLint _internalFormat;
        srGLsizei _width;
        srGLsizei _height;
        srGLint _border;
        srGLenum _format;
        srGLenum _type;
        std::vector<glsr::Models::Vector4d<srGLfloat>> _pixelData;

        std::map<srGLenum, srGLint> _parameters;
    };
} // namespace glsr::Services::Texture::Models

#endif // GLSR_TEXTURE_MODEL_H