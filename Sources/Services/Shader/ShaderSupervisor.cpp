#include "ShaderSupervisor.h"

#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <utility>

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Public/ResourceLimits.h>

#include "srglext.h"

#include "ProgramWorker.h"

using namespace glsr::Services::Shader;
using namespace glsr::Services::Shader::Models;
using namespace glsr::Services::Texture::Models;

ShaderSupervisor::ShaderSupervisor(std::shared_ptr<Texture::ITextureService> textureService)
    : _textureService(std::move(textureService))
{
}

srGLuint ShaderSupervisor::createProgram()
{
    srGLuint programId = ++_nextProgramId;
    programs[programId] = std::make_unique<ProgramWorker>(_textureService);

    return programId;
}

srGLuint ShaderSupervisor::createShader(srGLenum shaderType)
{
    srGLuint shaderId = ++_nextShaderId;
    shaders[shaderId] =
        std::make_shared<ShaderModel>(shaderType);

    return shaderId;
}

void ShaderSupervisor::shaderSource(srGLuint shader, srGLsizei count, const srGLchar *const *string, const srGLint *length)
{
    auto shaderIt = shaders.find(shader);
    assert(shaderIt != shaders.end() && "Shader not found");

    std::string sourceCode;
    for (srGLsizei i = 0; i < count; ++i)
    {
        if (length && length[i] > 0)
        {
            sourceCode.append(string[i], length[i]);
        }
        else
        {
            sourceCode += string[i];
        }
    }

    shaders[shader]->sourceCode = sourceCode;
}

void ShaderSupervisor::compileShader(srGLuint shader)
{
    auto shaderIt = shaders.find(shader);
    assert(shaderIt != shaders.end() && "Shader not found");

    auto shaderModel = shaderIt->second;

    // Compile the shader source code to SPIR-V
    auto eshLangType = _getGlslangShaderType(shaderModel->shaderType);
    glslang::TShader glslangShader(eshLangType);
    const char *sourceCodeCStr = shaderModel->sourceCode.c_str();
    glslangShader.setStrings(&sourceCodeCStr, 1);

    if (!glslangShader.parse(GetDefaultResources(), 330, false, EShMsgDefault))
    {
        std::string infoLog = glslangShader.getInfoLog();

        shaderModel->compileStatus[srGL_COMPILE_STATUS] = srGL_FALSE;
        shaderModel->infoLog = infoLog;
        return;
    }
    else
    {
        shaderModel->compileStatus[srGL_COMPILE_STATUS] = srGL_TRUE;
    }

    glslang::TProgram program;
    program.addShader(&glslangShader);

    if (!program.link(EShMsgDefault))
    {
        std::string infoLog = program.getInfoLog();

        shaderModel->compileStatus[srGL_COMPILE_STATUS] = srGL_FALSE;
        shaderModel->infoLog = infoLog;
        return;
    }
    else
    {
        shaderModel->compileStatus[srGL_COMPILE_STATUS] = srGL_TRUE;
    }

    std::vector<uint32_t> spirvCode;
    glslang::GlslangToSpv(*program.getIntermediate(eshLangType), spirvCode);

    shaderModel->spirvCode = std::move(spirvCode);
}

void ShaderSupervisor::getShaderiv(srGLuint shader, srGLenum pname, srGLint *params)
{
    auto shaderIt = shaders.find(shader);
    assert(shaderIt != shaders.end() && "Shader not found");

    auto shaderModel = shaderIt->second;

    auto statusIt = shaderModel->compileStatus.find(pname);
    if (statusIt != shaderModel->compileStatus.end())
    {
        *params = statusIt->second;
    }
    else
    {
        *params = srGL_FALSE; // Default to false if not set
    }
}

void ShaderSupervisor::getShaderInfoLog(srGLuint shader, srGLsizei bufSize, srGLsizei *length, srGLchar *infoLog)
{
    auto shaderIt = shaders.find(shader);
    assert(shaderIt != shaders.end() && "Shader not found");

    auto shaderModel = shaderIt->second;

    if (infoLog && bufSize > 0)
    {
        std::strncpy(infoLog, shaderModel->infoLog.c_str(), bufSize - 1);
        infoLog[bufSize - 1] = '\0'; // Ensure null-termination
    }

    if (length)
    {
        *length = static_cast<srGLsizei>(shaderModel->infoLog.size());
    }
}

void ShaderSupervisor::deleteShader(srGLuint shader)
{
    shaders.erase(shader);
}

void ShaderSupervisor::attachShader(srGLuint program, srGLuint shader)
{
    auto programIt = programs.find(program);
    assert(programIt != programs.end() && "Program not found");

    auto shaderIt = shaders.find(shader);
    assert(shaderIt != shaders.end() && "Shader not found");

    auto& programModel = programIt->second;
    auto& shaderModel = shaderIt->second;

    programModel->attachShader(shaderModel);
}

void ShaderSupervisor::linkProgram(srGLuint program)
{
    auto programIt = programs.find(program);
    assert(programIt != programs.end() && "Program not found");

    // For simplicity, we assume linking is always successful in this mock implementation.
    // In a real implementation, you would perform actual linking and set the link status accordingly.
}

void ShaderSupervisor::getProgramIv(srGLuint program, srGLenum pname, srGLint *params)
{
    auto programIt = programs.find(program);
    assert(programIt != programs.end() && "Program not found");

    // For simplicity, we assume the program is always linked successfully in this mock implementation.
    // In a real implementation, you would check the actual link status and set the params accordingly.
    if (pname == srGL_LINK_STATUS)
    {
        *params = srGL_TRUE; // Assume linking is successful
    }
    else
    {
        *params = 0; // Default value for other parameters
    }
}

void ShaderSupervisor::getProgramInfoLog(srGLuint program, srGLsizei bufSize, srGLsizei *length, srGLchar *infoLog)
{
    auto programIt = programs.find(program);
    assert(programIt != programs.end() && "Program not found");

    // For simplicity, we assume there are no errors in this mock implementation.
    // In a real implementation, you would retrieve the actual info log from the program.
    std::string log = "No errors.";

    if (infoLog && bufSize > 0)
    {
        std::strncpy(infoLog, log.c_str(), bufSize - 1);
        infoLog[bufSize - 1] = '\0'; // Ensure null-termination
    }

    if (length)
    {
        *length = static_cast<srGLsizei>(log.size());
    }
}

void ShaderSupervisor::useProgram(srGLuint program)
{
    assert(program == 0 || programs.find(program) != programs.end() && "Program not found");

    _boundProgram = program;
}

srGLint ShaderSupervisor::getUniformLocation(srGLuint program, const std::string &name)
{
    auto programIt = programs.find(program);
    assert(programIt != programs.end() && "Program not found");

    return programIt->second->getUniformLocation(name);
}

void ShaderSupervisor::uniform1i(srGLint location, srGLint value)
{
    auto programIt = programs.find(_boundProgram);
    assert(programIt != programs.end() && "Program not found");
    
    programIt->second->uniform1i(location, value);
}

void ShaderSupervisor::uniform3fv(srGLint location, srGLsizei count, const srGLfloat *value)
{
    auto programIt = programs.find(_boundProgram);
    assert(programIt != programs.end() && "Program not found");
    
    programIt->second->uniform3fv(location, count, value);
}

void ShaderSupervisor::uniformMatrix4fv(srGLint location, srGLsizei count, srGLboolean transpose, const srGLfloat *value)
{
    auto programIt = programs.find(_boundProgram);
    assert(programIt != programs.end() && "Program not found");
    
    programIt->second->uniformMatrix4fv(location, count, transpose, value);
}

void ShaderSupervisor::deleteProgram(srGLuint program)
{
    programs.erase(program);
}

srGLuint ShaderSupervisor::getBoundProgram() const
{
    return _boundProgram;
}

std::vector<OutputValue> ShaderSupervisor::runShader(srGLuint program, srGLenum shaderType, const std::vector<InputValue> &inputs)
{
    auto currentProgramIt = programs.find(program);
    assert(currentProgramIt != programs.end() && "Program not found");

    auto& programWorker = currentProgramIt->second;
    auto outputs = programWorker->runShader(shaderType, inputs);

    return outputs;
}

EShLanguage ShaderSupervisor::_getGlslangShaderType(srGLenum shaderType)
{
    switch (shaderType)
    {
    case srGL_VERTEX_SHADER:
        return EShLangVertex;
    case srGL_FRAGMENT_SHADER:
        return EShLangFragment;
    default:
        throw std::invalid_argument("Unsupported shader type");
    }
}
