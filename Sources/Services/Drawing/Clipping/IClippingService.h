#ifndef GLSR_SOURCES_SERVICES_DRAWING_CLIPPING_ICLIPPINGSERVICE_H
#define GLSR_SOURCES_SERVICES_DRAWING_CLIPPING_ICLIPPINGSERVICE_H

#include <vector>
#include <array>

#include "Services/Shader/Models/OutputValue.h"

namespace glsr::Services::Drawing::Clipping
{
    class IClippingService
    {
    public:
        virtual ~IClippingService() = default;

        virtual std::vector<std::array<std::vector<Shader::Models::OutputValue>, 3>> 
            clip(const std::array<std::vector<Shader::Models::OutputValue>, 3>& vertices, float nearPlaneWEpsilon) = 0;
    };
}

#endif // GLSR_SOURCES_SERVICES_DRAWING_CLIPPING_ICLIPPINGSERVICE_H