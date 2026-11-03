#include "Facade.h"

#include <cassert>

#include "Buffer/BufferService.h"
#include "Shader/ShaderSupervisor.h"
#include "Drawing/DrawingService.h"
#include "Texture/TextureService.h"
#include "Configuration/ConfigurationService.h"
#include "Drawing/Clipping/SutherlandHodgmanClippingService.h"

using namespace glsr::Services;
using namespace glsr::Services::Buffer;
using namespace glsr::Services::Shader;
using namespace glsr::Services::Drawing;
using namespace glsr::Services::Texture;
using namespace glsr::Services::Configuration;

Facade::Facade()
    : _buffersService(std::make_shared<BufferService>()),
      _textureService(std::make_shared<TextureService>()),
      _shaderService(std::make_shared<ShaderSupervisor>(_textureService)),
      _configurationService(std::make_shared<ConfigurationService>()),
      _clippingService(std::make_shared<SutherlandHodgmanClippingService>())
{
    _drawingService = std::make_shared<DrawingService>(_shaderService, _buffersService, _configurationService, _clippingService);
}

std::shared_ptr<Buffer::IBufferService> Facade::getBufferService()
{
    assert(_buffersService && "BufferService is not initialized");

    return _buffersService;
}

std::shared_ptr<Shader::IShaderService> Facade::getShaderService()
{
    assert(_shaderService && "ShaderService is not initialized");

    return _shaderService;
}

std::shared_ptr<Drawing::IDrawingService> Facade::getDrawingService()
{
    assert(_drawingService && "DrawingService is not initialized");

    return _drawingService;
}

std::shared_ptr<Texture::ITextureService> Facade::getTextureService()
{
    assert(_textureService && "TextureService is not initialized");

    return _textureService;
}

std::shared_ptr<Configuration::IConfigurationService> Facade::getConfigurationService()
{
    assert(_configurationService && "ConfigurationService is not initialized");

    return _configurationService;
}