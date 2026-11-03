#include "ProgramWorker.h"

#include <cassert>
#include <algorithm>

#include "srglext.h"

#include "SpirVm/SpirVmExtensions.h"
#include "SpirVm/SpirVmTextureExtensions.h"

using namespace glsr::Services::Shader;
using namespace glsr::Services::Shader::SpirVm;

ProgramWorker::ProgramWorker(std::shared_ptr<Texture::ITextureService> textureService)
    : _textureService(std::move(textureService))
{
    _context = spvm_context_initialize();
}

ProgramWorker::~ProgramWorker()
{
    spvm_context_deinitialize(_context);
}

void ProgramWorker::attachShader(std::shared_ptr<Models::ShaderModel> shaderModel)
{
    assert(shaderModel->spirvCode.size() > 0 && "Shader must be compiled before attaching to a program");

    // Store the shader model to prevent deleting it
    // even if nobody else holds a reference to it
    // (spvm_program_t takes just a raw pointer to the SPIR-V code, 
    // so we need to keep the shader model alive)
    _shaderModels[shaderModel->shaderType] = shaderModel;

    auto &spirvCode = shaderModel->spirvCode;
    spvm_source spv = reinterpret_cast<spvm_source>(spirvCode.data());

    _programs[shaderModel->shaderType] = spv;
}

std::vector<OutputValue> ProgramWorker::runShader(srGLenum shaderType, const std::vector<InputValue> &inputs)
{
    spvm_source spv = _programs[shaderType];
    spvm_program_t prog = spvm_program_create(_context, spv, _shaderModels[shaderType]->spirvCode.size());
    spvm_state_t state = spvm_state_create(prog);

    // Assign inpits to the shader state
    for (const auto &input : inputs)
    {
        switch (input.type)
        {
        case srGL_FLOAT:
            if (input.isNamed())
            {
                SpirVmExtensions::setInputByName(state, input.name, spvm_value_type_float, &input.value.floatValue);
            }
            else
            {
                SpirVmExtensions::setInputByLocation(state, input.location, spvm_value_type_float, &input.value.floatValue, 1);
            }
            break;
        case srGL_INT:
            if (input.isNamed())
            {
                SpirVmExtensions::setInputByName(state, input.name, spvm_value_type_int, &input.value.intValue);
            }
            else
            {
                SpirVmExtensions::setInputByLocation(state, input.location, spvm_value_type_int, &input.value.intValue, 1);
            }
            break;
        case srGL_FLOAT_VEC3:
            if (input.isNamed())
            {
                SpirVmExtensions::setInputByName(state, input.name, spvm_value_type_float, &input.value.floatVector3dValue);
            }
            else
            {
                SpirVmExtensions::setInputByLocation(state, input.location, spvm_value_type_float, &input.value.floatVector3dValue, 3);
            }
            break;
        case srGL_FLOAT_VEC2:
            if (input.isNamed())
            {
                SpirVmExtensions::setInputByName(state, input.name, spvm_value_type_float, &input.value.floatVector2dValue);
            }
            else
            {
                SpirVmExtensions::setInputByLocation(state, input.location, spvm_value_type_float, &input.value.floatVector2dValue, 2);
            }
            break;
        default:
            break;
        }
    }

    // Assign uniform values to the shader state
    for (const auto &[name, location] : _uniformLocations)
    {
        // Check if it's an 1i uniform
        auto valueIt = _uniform1iValues.find(location);
        if (valueIt != _uniform1iValues.end())
        {
            srGLint value = valueIt->second;
            SpirVmExtensions::setUniformUintValueByName(state, name, value);
            continue;
        }

        // Check if it's a 3fv uniform
        auto vectorIt = _uniform3fvValues.find(location);
        if (vectorIt != _uniform3fvValues.end())
        {
            const std::vector<srGLfloat> &values = vectorIt->second;
            SpirVmExtensions::setUniformVectorFloatByName(state, name, values.data());
            continue;
        }

        // Check if it's a matrix4fv uniform
        auto matrixIt = _uniformMatrix4fvValues.find(location);
        if (matrixIt != _uniformMatrix4fvValues.end())
        {
            const std::vector<srGLfloat> &values = matrixIt->second;
            SpirVmExtensions::setUniformMatrix4fvByName(state, name, values.data());
            continue;
        }
    }

    // Assign textures to the shader state
    auto samplerUniformTextureUnits = SpirVmTextureExtensions::findSampler2DUniformNames(state);
    for (const auto &[samplerUniformName, textureUnitNumber] : samplerUniformTextureUnits)
    {
        auto textureUnit = static_cast<srGLenum>(srGL_TEXTURE0 + textureUnitNumber);
        auto &textureModel = _textureService->getTexture(textureUnit, srGL_TEXTURE_2D);

        SpirVmTextureExtensions::setSampler2D(state, samplerUniformName, textureModel);
    }

    // Assign cubemap textures to the shader state
    auto samplerCubeUniformTextureUnits = SpirVmTextureExtensions::findSamplerCubeUniformNames(state);
    for (const auto &[samplerUniformName, textureUnitNumber] : samplerCubeUniformTextureUnits)
    {
        auto textureUnit = static_cast<srGLenum>(srGL_TEXTURE0 + textureUnitNumber);
        auto &textureModel = _textureService->getTexture(textureUnit, srGL_TEXTURE_CUBE_MAP);

        SpirVmTextureExtensions::setSamplerCube(state, samplerUniformName, textureModel);
    }

    // Execute the shader's entry point
    spvm_word mainLocation = spvm_state_get_result_location(state, const_cast<spvm_string>("main"));
    spvm_state_prepare(state, mainLocation);
    spvm_state_call_function(state);

    // Collect the shader outputs
    std::vector<OutputValue> outputs = SpirVmExtensions::collectShaderOutputs(state);

    spvm_state_delete(state);
    spvm_program_delete(prog);

    return outputs;
}

srGLint ProgramWorker::getUniformLocation(const std::string &name)
{
    // Check if the uniform location is already cached
    auto it = _uniformLocations.find(name);
    if (it != _uniformLocations.end())
    {
        return it->second;
    }

    // check if any program has the uniform with the given name
    for (const auto &[shaderType, spv] : _programs)
    {
        spvm_program_t prog = spvm_program_create(_context, spv, _shaderModels[shaderType]->spirvCode.size());
        auto state = spvm_state_create(prog);
        bool isUniformExists = SpirVmExtensions::isUniformExists(state, name);
        spvm_state_delete(state);
        spvm_program_delete(prog);

        if (isUniformExists)
        {
            // We cannot use location from the program,
            // since the location is specific to the shader
            // program and may cause conflicts when program have
            // different uniforms with the same location
            return _uniformLocations[name] = ++_nextUniformLocation;
        }
    }

    assert(false && "Uniform not found in any attached shader program");
    return static_cast<srGLint>(-1); // Uniform not found
}

void ProgramWorker::uniform1i(srGLint location, srGLint value)
{
    assert(std::find_if(_uniformLocations.begin(), _uniformLocations.end(),
                     [location](const auto &pair) { return pair.second == location; }) != _uniformLocations.end() &&
           "Uniform location not found");

    _uniform1iValues[location] = value;
}

void ProgramWorker::uniform3fv(srGLint location, srGLsizei count, const srGLfloat *value)
{
    assert(std::find_if(_uniformLocations.begin(), _uniformLocations.end(),
                     [location](const auto &pair) { return pair.second == location; }) != _uniformLocations.end() &&
           "Uniform location not found");

    _uniform3fvValues[location] = std::vector<srGLfloat>(value, value + count * 3);
}

void ProgramWorker::uniformMatrix4fv(srGLint location, srGLsizei count, srGLboolean transpose, const srGLfloat *value)
{
    assert(std::find_if(_uniformLocations.begin(), _uniformLocations.end(),
                     [location](const auto &pair) { return pair.second == location; }) != _uniformLocations.end() &&
           "Uniform location not found");

    // Store the matrix values in column-major order
    std::vector<srGLfloat> matrixValues(value, value + count * 16);
    if (transpose)
    {
        for (srGLsizei i = 0; i < count; ++i)
        {
            for (srGLsizei row = 0; row < 4; ++row)
            {
                for (srGLsizei col = row + 1; col < 4; ++col)
                {
                    std::swap(matrixValues[i * 16 + row * 4 + col], matrixValues[i * 16 + col * 4 + row]);
                }
            }
        }
    }

    _uniformMatrix4fvValues[location] = std::move(matrixValues);
}