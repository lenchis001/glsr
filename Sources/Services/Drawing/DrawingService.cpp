#include "DrawingService.h"

#include "Strategies/DrawArrays/DrawTriangleArraysStrategy.h"
#include "Strategies/DrawElements/DrawTriangleElementsStrategy.h"

using namespace glsr::Services::Drawing;
using namespace glsr::Services::Drawing::Strategies::DrawArrays;
using namespace glsr::Services::Drawing::Strategies::DrawElements;

DrawingService::DrawingService(
    std::shared_ptr<IShaderService> shaderService,
    std::shared_ptr<Buffer::IBufferService> buffersService,
    std::shared_ptr<IConfigurationService> configurationService,
    std::shared_ptr<IClippingService> clippingService)
    : _shaderService(std::move(shaderService)),
      _buffersService(std::move(buffersService)),
      _configurationService(std::move(configurationService)),
      _clippingService(std::move(clippingService)),
      _viewport(0, 0, 0, 0)
{
}

void DrawingService::drawArrays(srGLenum mode, srGLint first, srGLsizei count)
{
    DrawTriangleArraysStrategy drawStrategy(
        mode,
        first,
        count,
        _shaderService,
        _buffersService,
        _configurationService,
        _clippingService,
        _viewport);

    auto fragments = drawStrategy.processVertices();

    srGLboolean depthTestEnabled;
    _configurationService->getBooleanv(srGL_DEPTH_TEST, &depthTestEnabled);
    if (depthTestEnabled)
    {
        fragments = _depthTestFragments(fragments);
    }

    // todo: can be run in parallel with other fragment shader executions
    auto pixels = drawStrategy.processFragments(fragments);

    _updateFramebuffer(pixels);
}

void DrawingService::drawElements(srGLenum mode, srGLsizei count, srGLenum type, const void *indices)
{
    DrawTriangleElementsStrategy drawStrategy(
        mode,
        count,
        type,
        indices,
        _shaderService,
        _buffersService,
        _configurationService,
        _clippingService,
        _viewport);

    auto fragments = drawStrategy.processVertices();

    srGLboolean depthTestEnabled;
    _configurationService->getBooleanv(srGL_DEPTH_TEST, &depthTestEnabled);
    if (depthTestEnabled)
    {
        fragments = _depthTestFragments(fragments);
    }

    // todo: can be run in parallel with other fragment shader executions
    auto pixels = drawStrategy.processFragments(fragments);

    _updateFramebuffer(pixels);
}

void DrawingService::viewport(srGLint x, srGLint y, srGLsizei width, srGLsizei height)
{
    _viewport.x = x;
    _viewport.y = y;
    _viewport.width = width;
    _viewport.height = height;

    _framebuffer.resize(_viewport.width * _viewport.height * 4); // 4 bytes per pixel (RGBA)
}

void DrawingService::readPixels(srGLint x, srGLint y, srGLsizei width, srGLsizei height, srGLenum format, srGLenum type, void *data)
{
    if (format != srGL_RGB || type != srGL_UNSIGNED_BYTE)
    {
        throw std::runtime_error("Unsupported format or type in readPixels");
    }

    for (srGLsizei row = 0; row < height; ++row)
    {
        for (srGLsizei col = 0; col < width; ++col)
        {
            srGLint framebufferX = x + col;
            srGLint framebufferY = y + row;

            if (framebufferX >= 0 && framebufferX < _viewport.width && framebufferY >= 0 && framebufferY < _viewport.height)
            {
                size_t framebufferIndex = (framebufferY * _viewport.width + framebufferX) * 4;
                size_t dataIndex = (row * width + col) * 3;

                reinterpret_cast<srGLubyte *>(data)[dataIndex] = _framebuffer[framebufferIndex];
                reinterpret_cast<srGLubyte *>(data)[dataIndex + 1] = _framebuffer[framebufferIndex + 1];
                reinterpret_cast<srGLubyte *>(data)[dataIndex + 2] = _framebuffer[framebufferIndex + 2];
            }
        }
    }
}

void DrawingService::clearColor(srGLfloat red, srGLfloat green, srGLfloat blue, srGLfloat alpha)
{
    _clearColor = Vector4d<srGLfloat>(red, green, blue, alpha);
}

void DrawingService::clear(srGLbitfield mask)
{
    if (mask & srGL_COLOR_BUFFER_BIT)
    {
        // encode RGBA as one 32-bit integer for faster filling (little-endian: R is lowest byte)
        uint32_t color = (static_cast<uint32_t>(_clearColor.x * 255.0f)) |
                         (static_cast<uint32_t>(_clearColor.y * 255.0f) << 8) |
                         (static_cast<uint32_t>(_clearColor.z * 255.0f) << 16) |
                         (static_cast<uint32_t>(_clearColor.w * 255.0f) << 24);

        // Fill the framebuffer with the clear color
        std::fill(
            reinterpret_cast<uint32_t *>(_framebuffer.data()),
            reinterpret_cast<uint32_t *>(_framebuffer.data() + _framebuffer.size()),
            color);
    }

    if (mask & srGL_DEPTH_BUFFER_BIT)
    {
        _depthMatrix.clear();
    }
}

std::vector<Fragment> DrawingService::_depthTestFragments(const std::vector<Fragment> &fragments)
{
    std::vector<Fragment> depthTestedFragments;

    for (const auto &fragment : fragments)
    {
        auto it = _depthMatrix.find(fragment.position);
        if (it == _depthMatrix.end() || fragment.depth < it->second)
        {
            _depthMatrix[fragment.position] = fragment.depth;
            depthTestedFragments.push_back(fragment);
        }
    }

    return depthTestedFragments;
}

void DrawingService::_updateFramebuffer(std::map<Point<srGLsizei>, Vector4d<srGLfloat>> &depthMatrix)
{
    for (const auto &entry : depthMatrix)
    {
        const Point<srGLsizei> &position = entry.first;
        const Vector4d<srGLfloat> &color = entry.second;
        assert(position.x >= 0 && position.x < _viewport.width && position.y >= 0 && position.y < _viewport.height && "Position out of viewport bounds");

        size_t framebufferIndex = (position.y * _viewport.width + position.x) * 4;
        assert(framebufferIndex + 3 < _framebuffer.size() && "Framebuffer index out of bounds");

        srGLboolean isBlendEnabled;
        _configurationService->getBooleanv(srGL_BLEND, &isBlendEnabled);

        Vector4d<srGLfloat> finalColor = color;
        if (isBlendEnabled)
        {
            // GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA blending
            const Vector4d<srGLfloat> &dstColor = Vector4d<srGLfloat>(
                _framebuffer[framebufferIndex] / 255.0f,
                _framebuffer[framebufferIndex + 1] / 255.0f,
                _framebuffer[framebufferIndex + 2] / 255.0f,
                _framebuffer[framebufferIndex + 3] / 255.0f);

            finalColor.x = finalColor.x * finalColor.w + dstColor.x * (1.0f - finalColor.w);
            finalColor.y = finalColor.y * finalColor.w + dstColor.y * (1.0f - finalColor.w);
            finalColor.z = finalColor.z * finalColor.w + dstColor.z * (1.0f - finalColor.w);
            finalColor.w = finalColor.w + dstColor.w * (1.0f - finalColor.w);
        }

        _framebuffer[framebufferIndex] = static_cast<srGLubyte>(finalColor.x * 255.0f);
        _framebuffer[framebufferIndex + 1] = static_cast<srGLubyte>(finalColor.y * 255.0f);
        _framebuffer[framebufferIndex + 2] = static_cast<srGLubyte>(finalColor.z * 255.0f);
        _framebuffer[framebufferIndex + 3] = static_cast<srGLubyte>(finalColor.w * 255.0f);
    }
}