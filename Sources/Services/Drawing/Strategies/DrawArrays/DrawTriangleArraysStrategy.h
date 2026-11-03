#ifndef GLSR_DRAW_TRIANGLE_ARRAYS_STRATEGY
#define GLSR_DRAW_TRIANGLE_ARRAYS_STRATEGY

#include "Services/Drawing/Strategies/IDrawStrategy.h"

#include <memory>
#include <array>

#include "srtypes.h"

#include "Services/Shader/IShaderService.h"
#include "Services/Buffer/IBufferService.h"
#include "Services/Configuration/IConfigurationService.h"
#include "Services/Drawing/Clipping/IClippingService.h"

#include "Services/Drawing/Models/Rect.h"
#include "Services/Drawing/Models/Point.h"

#include "Models/ViewTriangle.h"

#include "Services/Drawing/Strategies/DrawStrategyBase.h"

using namespace glsr::Services::Shader;
using namespace glsr::Services::Buffer;
using namespace glsr::Services::Drawing::Clipping;

using namespace glsr::Models;
using namespace glsr::Services::Shader::Models;
using namespace glsr::Services::Drawing::Models;
using namespace glsr::Services::Configuration;

using namespace glsr::Services::Drawing::Strategies::DrawArrays::Models;

namespace glsr::Services::Drawing::Strategies::DrawArrays
{
    class DrawTriangleArraysStrategy : public DrawStrategyBase, public IDrawStrategy
    {
    public:
        DrawTriangleArraysStrategy(
            srGLenum mode,
            srGLint first,
            srGLsizei count,
            std::shared_ptr<IShaderService> shaderService,
            std::shared_ptr<IBufferService> buffersService,
            std::shared_ptr<IConfigurationService> configurationService,
            std::shared_ptr<IClippingService> clippingService,
            const Rect &viewport);

        std::vector<Fragment> processVertices() override;

        std::map<Drawing::Models::Point<srGLsizei>, glsr::Models::Vector4d<srGLfloat>> processFragments(const std::vector<Fragment>& fragments) override;

    protected:
        virtual std::vector<std::vector<InputValue>> _collectVertexShaderInputs();

    private:
        std::vector<Fragment> _rasterize(const std::vector<ViewTriangle<srGLsizei>> &viewTriangles);
        std::vector<ViewTriangle<srGLsizei>> _toViewTriangles(const std::vector<std::vector<OutputValue>> &vertexShaderOutputs);
        
        std::vector<Fragment> _rasterizeTriangle(const ViewTriangle<srGLsizei> &vertexShaderTriangle);
        std::vector<InputValue> _interpolateOutputs(
            const std::array<std::vector<OutputValue>, 3> &vertexShaderCustomOutputs,
            float w0, float w1, float w2);

        std::vector<ViewTriangle<srGLsizei>> _cullFaceTest(const std::vector<ViewTriangle<srGLsizei>> &triangles);

        const Rect &_viewport;
        std::shared_ptr<IConfigurationService> _configurationService;
        std::shared_ptr<IClippingService> _clippingService;
    };
}

#endif // GLSR_DRAW_TRIANGLE_ARRAYS_STRATEGY