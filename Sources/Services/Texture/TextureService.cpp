#include "TextureService.h"

#include <cassert>
#include <cstdint>

using namespace glsr::Services::Texture;
using namespace glsr::Models;

TextureService::TextureService()
    : _nextTextureId(0), _activeTextureUnit(srGL_TEXTURE0)
{
}

void TextureService::genTextures(srGLsizei n, srGLuint *textures)
{
    for (srGLsizei i = 0; i < n; ++i)
    {
        srGLuint textureId = ++_nextTextureId;
        textures[i] = textureId;

        TextureModel textureModel;
        textureModel.setParameter(srGL_TEXTURE_WRAP_S, srGL_REPEAT);
        textureModel.setParameter(srGL_TEXTURE_WRAP_T, srGL_REPEAT);
        textureModel.setParameter(srGL_TEXTURE_WRAP_R, srGL_REPEAT);
        textureModel.setParameter(srGL_TEXTURE_MIN_FILTER, srGL_NEAREST_MIPMAP_LINEAR);
        textureModel.setParameter(srGL_TEXTURE_MAG_FILTER, srGL_LINEAR);

        _textures.insert_or_assign(textureId, textureModel);
    }
}

void TextureService::activeTexture(srGLenum texture)
{
    assert(texture >= srGL_TEXTURE0 && texture <= srGL_TEXTURE31 && "Invalid texture unit!");

    _activeTextureUnit = texture;
}

void TextureService::bindTexture(srGLenum target, srGLuint texture)
{
    assert(texture == 0 || _textures.find(texture) != _textures.end() && "Texture ID not found!");

    _boundTextures[_activeTextureUnit][target] = texture;
}

void TextureService::texParameteri(srGLenum target, srGLenum pname, srGLint param)
{
    auto unitIt = _boundTextures.find(_activeTextureUnit);
    assert(unitIt != _boundTextures.end() && "No texture bound to the active texture unit!");

    auto it = unitIt->second.find(target);
    assert(it != unitIt->second.end() && "No texture bound to the specified target!");

    srGLuint textureId = it->second;

    auto textureIt = _textures.find(textureId);
    assert(textureIt != _textures.end() && "Texture ID not found!");

    textureIt->second.setParameter(pname, param);
}

void TextureService::texImage2D(
    srGLenum target,
    srGLint level,
    srGLint internalFormat,
    srGLsizei width,
    srGLsizei height,
    srGLint border,
    srGLenum format,
    srGLenum type,
    const void *data)
{
    auto unitIt = _boundTextures.find(_activeTextureUnit);
    assert(unitIt != _boundTextures.end() && "No texture bound to the active texture unit!");

    auto it = unitIt->second.find(target >= srGL_TEXTURE_CUBE_MAP_POSITIVE_X && target <= srGL_TEXTURE_CUBE_MAP_NEGATIVE_Z ? srGL_TEXTURE_CUBE_MAP : target);
    assert(it != unitIt->second.end() && "No texture bound to the specified target!");

    srGLuint textureId = it->second;

    auto textureIt = _textures.find(textureId);
    assert(textureIt != _textures.end() && "Texture ID not found!");

    textureIt->second.patchData(
        target,
        level,
        0,
        0,
        width,
        height,
        format,
        type,
        data);
}

void TextureService::texSubImage2D(
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
    auto unitIt = _boundTextures.find(_activeTextureUnit);
    assert(unitIt != _boundTextures.end() && "No texture bound to the active texture unit!");

    auto it = unitIt->second.find(target);
    assert(it != unitIt->second.end() && "No texture bound to the specified target!");

    srGLuint textureId = it->second;

    auto textureIt = _textures.find(textureId);
    assert(textureIt != _textures.end() && "Texture ID not found!");

    TextureModel &textureModel = textureIt->second;

    textureModel.patchData(
        target,
        level,
        xoffset,
        yoffset,
        width,
        height,
        format,
        type,
        data);
}

TextureModel &TextureService::getTexture(srGLenum textureUnit, srGLenum target)
{
    auto unitIt = _boundTextures.find(textureUnit);
    assert(unitIt != _boundTextures.end() && "No texture bound to the specified texture unit!");

    auto it = unitIt->second.find(target);
    assert(it != unitIt->second.end() && "No texture bound to the specified target!");

    srGLuint textureId = it->second;

    auto textureIt = _textures.find(textureId);
    assert(textureIt != _textures.end() && "Texture ID not found!");

    return textureIt->second;
}

void TextureService::deleteTextures(srGLsizei n, const srGLuint *textures)
{
    for (srGLsizei i = 0; i < n; ++i)
    {
        srGLuint textureId = textures[i];
        _textures.erase(textureId);
    }
}
