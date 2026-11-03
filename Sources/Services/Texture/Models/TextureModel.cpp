#include "TextureModel.h"

using namespace glsr::Services::Texture::Models;

TextureModel::TextureModel()
    : _levelOfDetail(0),
      _internalFormat(0),
      _width(0),
      _height(0),
      _border(0),
      _format(0),
      _type(0),
      _parameters()
{
}

void TextureModel::patchData(
    srGLenum target,
    srGLint level,
    srGLint xoffset,
    srGLint yoffset,
    srGLsizei width,
    srGLsizei height,
    srGLenum format,
    srGLenum type,
    const void *data)
{
    assert((_levelOfDetail == 0 || _levelOfDetail == level) && "Mismatched mipmap level for patchData");
    assert((_format == 0 || _format == format) && "Mismatched format for patchData");
    assert((_type == 0 || _type == type) && "Mismatched type for patchData");

    assert(xoffset >= 0 && yoffset >= 0 && "Negative offsets are not allowed");
    assert(((yoffset == 0 || _height == 0) || (yoffset + height <= _height)) && "Subimage exceeds texture dimensions");
    assert(((xoffset == 0 || _width == 0) || (xoffset + width <= _width)) && "Subimage exceeds texture dimensions");

    _levelOfDetail = level;
    _format = format;
    _type = type;
    if (xoffset == 0 && yoffset == 0)
    {
        _width = width;
        _height = height;
    }

    size_t sideOffset = 0;
    if (target >= srGL_TEXTURE_CUBE_MAP_POSITIVE_X && target <= srGL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
    {
        // Calculate the offset for the specific cubemap face
        sideOffset = static_cast<size_t>(target - srGL_TEXTURE_CUBE_MAP_POSITIVE_X) * _width * _height;
    }

    if (xoffset == 0 && yoffset == 0)
    {
        if (target >= srGL_TEXTURE_CUBE_MAP_POSITIVE_X && target <= srGL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
        {
            _pixelData.resize(static_cast<size_t>(_width) * _height * 6); // 6 faces for cubemap
        }
        else
        {
            _pixelData.resize(static_cast<size_t>(_width) * _height);
        }
    }

    std::vector<glsr::Models::Vector4d<srGLfloat>> newPixelData = _decodePixels(
        reinterpret_cast<const uint8_t *>(data),
        width,
        height);

    for (srGLsizei row = 0; row < height; ++row)
    {
        for (srGLsizei col = 0; col < width; ++col)
        {
            size_t destIndex = sideOffset + (yoffset + row) * _width + (xoffset + col);
            size_t srcIndex = row * width + col;
            assert(destIndex >= 0 && destIndex < _pixelData.size() && "Destination index out of bounds");
            _pixelData[destIndex] = newPixelData[srcIndex];
        }
    }
}

srGLint TextureModel::getLevel() const { return _levelOfDetail; }
srGLint TextureModel::getInternalFormat() const { return _internalFormat; }
srGLsizei TextureModel::getWidth() const { return _width; }
srGLsizei TextureModel::getHeight() const { return _height; }
srGLint TextureModel::getBorder() const { return _border; }
srGLenum TextureModel::getFormat() const { return _format; }
srGLenum TextureModel::getType() const { return _type; }

const std::vector<glsr::Models::Vector4d<srGLfloat>> &TextureModel::getPixelData() const
{
    return _pixelData;
}

srGLint TextureModel::getParameter(srGLenum key, srGLint defaultValue) const
{
    auto it = _parameters.find(key);
    return it != _parameters.end() ? it->second : defaultValue;
}

void TextureModel::setParameter(srGLenum key, srGLint value)
{
    _parameters[key] = value;
}

std::vector<glsr::Models::Vector4d<srGLfloat>> TextureModel::_decodePixels(
    const uint8_t *bytes,
    srGLsizei pixelWidth,
    srGLsizei pixelHeight) const
{
    const srGLsizei decodeWidth = pixelWidth < 0 ? _width : pixelWidth;
    const srGLsizei decodeHeight = pixelHeight < 0 ? _height : pixelHeight;
    std::vector<glsr::Models::Vector4d<srGLfloat>> pixels(static_cast<size_t>(decodeWidth) * decodeHeight);

    if (!bytes)
    {
        return pixels;
    }

    assert(_type == srGL_UNSIGNED_BYTE && "Unsupported texture data type");
    assert((_format == srGL_RGB || _format == srGL_RGBA) && "Unsupported texture format");

    srGLsizei channels = _format == srGL_RGBA ? 4 : 3;

    for (size_t i = 0; i < pixels.size(); ++i)
    {
        const uint8_t *texel = bytes + i * channels;
        pixels[i].x = texel[0] / 255.0f;
        pixels[i].y = texel[1] / 255.0f;
        pixels[i].z = texel[2] / 255.0f;
        pixels[i].w = channels == 4 ? texel[3] / 255.0f : 1.0f;
    }

    return pixels;
}