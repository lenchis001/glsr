#ifndef GLSR_FACADE_H
#define GLSR_FACADE_H

#include "IFacade.h"

#include <memory>

#include "Services/Drawing/Clipping/IClippingService.h"

using namespace glsr::Services::Drawing::Clipping;

namespace glsr::Services
{
    class Facade : public IFacade
    {
    public:
        Facade();

        std::shared_ptr<Buffer::IBufferService> getBufferService() override;

        std::shared_ptr<Shader::IShaderService> getShaderService() override;

        std::shared_ptr<Drawing::IDrawingService> getDrawingService() override;

        std::shared_ptr<Texture::ITextureService> getTextureService() override;

        std::shared_ptr<Configuration::IConfigurationService> getConfigurationService() override;
    private:
        std::shared_ptr<Buffer::IBufferService> _buffersService;
        std::shared_ptr<Texture::ITextureService> _textureService;
        std::shared_ptr<Shader::IShaderService> _shaderService;
        std::shared_ptr<Drawing::IDrawingService> _drawingService;
        std::shared_ptr<Configuration::IConfigurationService> _configurationService;

        std::shared_ptr<Drawing::Clipping::IClippingService> _clippingService;
    };
}

#endif // GLSR_FACADE_H