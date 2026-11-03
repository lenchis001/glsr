#ifndef GLSR_SOURCES_SERVICES_CONFIGURATION_ICONFIGURATIONSERVICE_H
#define GLSR_SOURCES_SERVICES_CONFIGURATION_ICONFIGURATIONSERVICE_H

#include <cstdint>
#include "srtypes.h"

namespace glsr::Services::Configuration
{
    class IConfigurationService
    {
    public:
        virtual ~IConfigurationService() = default;

        virtual void enable(srGLenum cap) = 0;

        virtual void disable(srGLenum cap) = 0;

        virtual void getBooleanv(srGLenum pname, srGLboolean *data) const = 0;

        virtual void blendFunc(srGLenum sfactor, srGLenum dfactor) = 0;

        virtual void cullFace(srGLenum mode) = 0;

        virtual void getIntegerv(srGLenum pname, srGLint *data) const = 0;
    };
}

#endif // GLSR_SOURCES_SERVICES_CONFIGURATION_ICONFIGURATIONSERVICE_H