#ifndef GLSR_IFACADE_H
#define GLSR_IFACADE_H

#include <memory>

#include "Buffer/IBufferService.h"
#include "Shader/IShaderService.h"
#include "Drawing/IDrawingService.h"
#include "Texture/ITextureService.h"
#include "Configuration/IConfigurationService.h"

namespace glsr::Services
{
    class IFacade
    {
    public:
        virtual ~IFacade() = default;
        
        virtual std::shared_ptr<Buffer::IBufferService> getBufferService() = 0;

        virtual std::shared_ptr<Shader::IShaderService> getShaderService() = 0;

        virtual std::shared_ptr<Drawing::IDrawingService> getDrawingService() = 0;

        virtual std::shared_ptr<Texture::ITextureService> getTextureService() = 0;

        virtual std::shared_ptr<Configuration::IConfigurationService> getConfigurationService() = 0;

        static IFacade& getInstance();
    };
}

#endif // GLSR_IFACADE_H