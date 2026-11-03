#ifndef GLSR_SOURCES_SERVICES_DRAWING_CLIPPING_SUTHERLANDHODGMANCLIPPINGSERVICE_H
#define GLSR_SOURCES_SERVICES_DRAWING_CLIPPING_SUTHERLANDHODGMANCLIPPINGSERVICE_H

#include "IClippingService.h"

using namespace glsr::Services::Shader::Models;

namespace glsr::Services::Drawing::Clipping
{
    class SutherlandHodgmanClippingService : public IClippingService
    {
    public:
        std::vector<std::array<std::vector<OutputValue>, 3>>
        clip(const std::array<std::vector<Shader::Models::OutputValue>, 3> &vertices, float nearPlaneWEpsilon) override;

    private:
        static float _getClipSpaceW(const std::vector<OutputValue> &vertexOutputs);
        std::vector<OutputValue> _lerpVertexOutputs(
            const std::vector<OutputValue> &a,
            const std::vector<OutputValue> &b,
            float t);
    };
}

#endif // GLSR_SOURCES_SERVICES_DRAWING_CLIPPING_SUTHERLANDHODGMANCLIPPINGSERVICE_H