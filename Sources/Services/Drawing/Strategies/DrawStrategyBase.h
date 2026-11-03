#ifndef GLSR_DRAW_STRATEGY_BASE_H
#define GLSR_DRAW_STRATEGY_BASE_H

#include <vector>
#include <memory>

#include "Services/Shader/Models/InputValue.h"
#include "Services/Buffer/IBufferService.h"
#include "Services/Shader/IShaderService.h"

using namespace glsr::Services::Buffer;
using namespace glsr::Services::Shader;
using namespace glsr::Services::Shader::Models;

namespace glsr::Services::Drawing::Strategies
{
    class DrawStrategyBase
    {
    public:
        DrawStrategyBase(
            srGLsizei firstVertexIndex,
            srGLsizei vertexCount,
            std::shared_ptr<IBufferService> buffersService,
            std::shared_ptr<IShaderService> shaderService);

    protected:
        std::vector<std::vector<InputValue>> _collectArrayVertexShaderInputs();
        std::vector<std::vector<InputValue>> _collectElementVertexShaderInputs();

        std::vector<std::vector<OutputValue>> _runVertexShader(const std::vector<std::vector<InputValue>> &inputs);

        std::vector<OutputValue> _runFragmentShader(const std::vector<InputValue> &fragmentShaderInputs);

        const std::shared_ptr<IBufferService> _buffersService;
        const std::shared_ptr<IShaderService> _shaderService;

        const srGLsizei _firstVertexIndex;
        const srGLsizei _vertexCount;

        const srGLuint _boundVaoId;
        const srGLuint _boundProgramId;
    };
}

#endif // GLSR_DRAW_STRATEGY_BASE_H