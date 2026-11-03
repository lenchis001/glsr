#include "DrawTriangleElementsStrategy.h"

#include <cassert>

using namespace glsr::Services::Drawing::Strategies::DrawElements;

DrawTriangleElementsStrategy::DrawTriangleElementsStrategy(
    srGLenum mode,
    srGLsizei count,
    srGLenum type,
    const void *indices,
    std::shared_ptr<glsr::Services::Shader::IShaderService> shaderService,
    std::shared_ptr<glsr::Services::Buffer::IBufferService> buffersService,
    std::shared_ptr<glsr::Services::Configuration::IConfigurationService> configurationService,
    std::shared_ptr<glsr::Services::Drawing::Clipping::IClippingService> clippingService,
    const Rect &viewport)
    : DrawTriangleArraysStrategy(
          mode,
          0,
          count,
          std::move(shaderService),
          std::move(buffersService),
          std::move(configurationService),
          std::move(clippingService),
          viewport)
{
    assert(type == srGL_UNSIGNED_INT && "Unsupported index type in DrawTriangleElementsStrategy");
}

std::vector<std::vector<InputValue>> DrawTriangleElementsStrategy::_collectVertexShaderInputs()
{
    return _collectElementVertexShaderInputs();
}