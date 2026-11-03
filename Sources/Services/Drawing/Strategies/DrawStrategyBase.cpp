#include "DrawStrategyBase.h"

#include <cstring>

#include "srglext.h"

#include "Models/Vector2d.h"
#include "Models/Vector3d.h"

using namespace glsr::Services::Drawing::Strategies;

using namespace glsr::Models;

DrawStrategyBase::DrawStrategyBase(
    srGLsizei firstVertexIndex,
    srGLsizei vertexCount,
    std::shared_ptr<IBufferService> buffersService,
    std::shared_ptr<IShaderService> shaderService)
    : _firstVertexIndex(firstVertexIndex),
      _vertexCount(vertexCount),
      _buffersService(std::move(buffersService)),
      _shaderService(std::move(shaderService)),
      // Store the currently bound VAO and program IDs for later use
      // in multiple draw calls or shader executions.
      _boundVaoId(_buffersService->getBoundVao()),
      _boundProgramId(_shaderService->getBoundProgram())
{
}

std::vector<std::vector<InputValue>> DrawStrategyBase::_collectArrayVertexShaderInputs()
{
    auto boundVao = _buffersService->getVao(_boundVaoId);

    std::vector<std::vector<InputValue>> inputs;
    for (auto i = _firstVertexIndex; i < _firstVertexIndex + _vertexCount; ++i)
    {
        std::vector<InputValue> input;

        for (const auto &[attribIndex, attribPointer] : boundVao.attribPointers)
        {
            auto attribData = _buffersService->getBufferedData(attribPointer.bufferId);
            auto attribSize = attribPointer.size;
            auto attribType = attribPointer.type;
            auto attribNormalized = attribPointer.normalized;
            auto attribStride = attribPointer.stride;

            auto stepSize = attribStride == 0 ? attribSize : attribStride / sizeof(srGLfloat);

            Value value;
            srGLenum inputType;
            switch (attribType)
            {
            case srGL_FLOAT:
            {
                size_t pointerOffset = reinterpret_cast<size_t>(attribPointer.pointer) / sizeof(srGLfloat);
                srGLfloat *floatData =
                    reinterpret_cast<srGLfloat *>(attribData.data.data()) +
                    i * stepSize +
                    pointerOffset;
                if (attribSize == 3)
                {
                    memcpy(&value.floatVector3dValue, floatData, sizeof(Vector3d<srGLfloat>));
                    inputType = srGL_FLOAT_VEC3;
                }
                else if (attribSize == 2)
                {
                    memcpy(&value.floatVector2dValue, floatData, sizeof(Vector2d<srGLfloat>));
                    inputType = srGL_FLOAT_VEC2;
                }
                else
                {
                    memcpy(&value.floatValue, floatData, sizeof(srGLfloat));
                    inputType = srGL_FLOAT;
                }
                break;
            }
            case srGL_INT:
            {
                size_t pointerOffset = reinterpret_cast<size_t>(attribPointer.pointer) / sizeof(srGLint);
                srGLint *intData =
                    reinterpret_cast<srGLint *>(attribData.data.data()) +
                    i * stepSize +
                    pointerOffset;
                if (attribSize == 3)
                {
                    memcpy(&value.intVector3dValue, intData, sizeof(Vector3d<srGLint>));
                    inputType = srGL_INT_VEC3;
                }
                else
                {
                    memcpy(&value.intValue, intData, sizeof(srGLint));
                    inputType = srGL_INT;
                }
                break;
            }
            default:
                assert(false && "Unsupported attribute type");
                break;
            }

            input.push_back(InputValue(attribIndex, inputType, value));
        }

        inputs.push_back(input);
    }

    return inputs;
}

std::vector<std::vector<InputValue>> DrawStrategyBase::_collectElementVertexShaderInputs()
{
    auto boundVao = _buffersService->getVao(_boundVaoId);
    auto elementArrayBufferId = boundVao.elementsBufferId;

    const auto &elementArrayData = _buffersService->getBufferedData(elementArrayBufferId);

    std::vector<std::vector<InputValue>> inputs;
    for (auto i = 0; i < _vertexCount; ++i)
    {
        std::vector<InputValue> input;

        // Get the index from the element array buffer
        srGLuint index;
        memcpy(&index, elementArrayData.data.data() + i * sizeof(srGLuint), sizeof(srGLuint));

        // Now collect the vertex attributes for this index
        for (const auto &[attribIndex, attribPointer] : boundVao.attribPointers)
        {
            if (attribIndex == srGL_ELEMENT_ARRAY_BUFFER)
                continue; // Skip the element array buffer itself

            const auto &attribData = _buffersService->getBufferedData(attribPointer.bufferId);
            const auto &attribSize = attribPointer.size;
            const auto &attribType = attribPointer.type;
            const auto &attribNormalized = attribPointer.normalized;
            const auto &attribStride = attribPointer.stride;

            auto stepSize = attribStride == 0 ? attribSize : attribStride / sizeof(srGLfloat);

            Value value;
            srGLenum inputType;
            switch (attribType)
            {
            case srGL_FLOAT:
            {
                size_t pointerOffset = reinterpret_cast<size_t>(attribPointer.pointer) / sizeof(srGLfloat);
                const srGLfloat *floatData =
                    reinterpret_cast<const srGLfloat *>(attribData.data.data()) +
                    index * stepSize +
                    pointerOffset;
                if (attribSize == 3)
                {
                    memcpy(&value.floatVector3dValue, floatData, sizeof(Vector3d<srGLfloat>));
                    inputType = srGL_FLOAT_VEC3;
                }
                else if (attribSize == 2)
                {
                    memcpy(&value.floatVector2dValue, floatData, sizeof(Vector2d<srGLfloat>));
                    inputType = srGL_FLOAT_VEC2;
                }
                else
                {
                    memcpy(&value.floatValue, floatData, sizeof(srGLfloat));
                    inputType = srGL_FLOAT;
                }
                break;
            }
            case srGL_INT:
            {
                size_t pointerOffset = reinterpret_cast<size_t>(attribPointer.pointer) / sizeof(srGLint);
                const srGLint *intData =
                    reinterpret_cast<const srGLint *>(attribData.data.data()) +
                    index * stepSize +
                    pointerOffset;
                if (attribSize == 3)
                {
                    memcpy(&value.intVector3dValue, intData, sizeof(Vector3d<srGLint>));
                    inputType = srGL_INT_VEC3;
                }
                else
                {
                    memcpy(&value.intValue, intData, sizeof(srGLint));
                    inputType = srGL_INT;
                }
                break;
            }
            default:
                assert(false && "Unsupported attribute type");
                break;
            }

            input.push_back(InputValue(attribIndex, inputType, value));
        }

        inputs.push_back(input);
    }

    return inputs;
}

std::vector<std::vector<OutputValue>> DrawStrategyBase::_runVertexShader(const std::vector<std::vector<InputValue>> &inputs)
{
    std::vector<std::vector<OutputValue>> shaderOutputs;
    for (const auto &input : inputs)
    {
        auto result = _shaderService->runShader(_boundProgramId, srGL_VERTEX_SHADER, input);
        shaderOutputs.push_back(result);
    }

    return shaderOutputs;
}

std::vector<OutputValue> DrawStrategyBase::_runFragmentShader(const std::vector<InputValue> &fragmentShaderInputs)
{
    return _shaderService->runShader(_boundProgramId, srGL_FRAGMENT_SHADER, fragmentShaderInputs);
}
