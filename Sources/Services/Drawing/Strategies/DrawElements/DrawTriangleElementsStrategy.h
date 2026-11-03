#ifndef SRGL_DRAW_TRIANGLE_ELEMENTS_STRATEGY_H
#define SRGL_DRAW_TRIANGLE_ELEMENTS_STRATEGY_H

#include "Services/Drawing/Strategies/DrawArrays/DrawTriangleArraysStrategy.h"

using namespace glsr::Services::Drawing::Strategies::DrawArrays;

namespace glsr::Services::Drawing::Strategies::DrawElements
{
    class DrawTriangleElementsStrategy : public DrawTriangleArraysStrategy
    {
    public:
        DrawTriangleElementsStrategy(
            srGLenum mode,
            srGLsizei count,
            srGLenum type,
            const void *indices,
            std::shared_ptr<glsr::Services::Shader::IShaderService> shaderService,
            std::shared_ptr<glsr::Services::Buffer::IBufferService> buffersService,
            std::shared_ptr<glsr::Services::Configuration::IConfigurationService> configurationService,
            std::shared_ptr<glsr::Services::Drawing::Clipping::IClippingService> clippingService,
            const Rect &viewport);

    protected:
        std::vector<std::vector<InputValue>> _collectVertexShaderInputs() override;
    };
}

#endif // SRGL_DRAW_TRIANGLE_ELEMENTS_STRATEGY_H