#include "ShaderModel.h"

using namespace glsr::Services::Shader::Models;

ShaderModel::ShaderModel(srGLenum shaderType)
    : shaderType(shaderType), sourceCode(""), spirvCode(), compileStatus(), infoLog("")
{
}