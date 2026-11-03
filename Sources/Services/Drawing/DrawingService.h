#ifndef GLSR_DRAWING_SERVICE_H
#define GLSR_DRAWING_SERVICE_H

#include "IDrawingService.h"

#include <memory>
#include <vector>
#include <map>

#include "Services/Shader/IShaderService.h"
#include "Services/Buffer/IBufferService.h"
#include "Services/Configuration/IConfigurationService.h"
#include "Services/Drawing/Clipping/IClippingService.h"

#include "Models/Vector4d.h"

#include "Models/Rect.h"
#include "Models/Point.h"
#include "Models/Fragment.h"

using namespace glsr::Models;

using namespace glsr::Services::Shader;
using namespace glsr::Services::Buffer;
using namespace glsr::Services::Configuration;
using namespace glsr::Services::Drawing::Clipping;

using namespace glsr::Services::Drawing::Models;

namespace glsr::Services::Drawing
{
    class DrawingService : public IDrawingService
    {
    public:
        DrawingService(
            std::shared_ptr<IShaderService> shaderService,
            std::shared_ptr<Buffer::IBufferService> buffersService,
            std::shared_ptr<IConfigurationService> configurationService,
            std::shared_ptr<IClippingService> clippingService);

        void drawArrays(srGLenum mode, srGLint first, srGLsizei count) override;

        void drawElements(srGLenum mode, srGLsizei count, srGLenum type, const void *indices) override;

        void viewport(srGLint x, srGLint y, srGLsizei width, srGLsizei height) override;

        void readPixels(srGLint x, srGLint y, srGLsizei width, srGLsizei height, srGLenum format, srGLenum type, void *data) override;

        void clearColor(srGLfloat red, srGLfloat green, srGLfloat blue, srGLfloat alpha) override;

        void clear(srGLbitfield mask) override;

    private:
        std::vector<Fragment> _depthTestFragments(const std::vector<Fragment> &fragments);
        void _updateFramebuffer(std::map<Point<srGLsizei>, Vector4d<srGLfloat>> &depthMatrix);

        std::shared_ptr<IShaderService> _shaderService;
        std::shared_ptr<Buffer::IBufferService> _buffersService;
        std::shared_ptr<IConfigurationService> _configurationService;
        std::shared_ptr<IClippingService> _clippingService;

        std::map<Point<srGLsizei>, float> _depthMatrix;
        std::vector<srGLubyte> _framebuffer;

        Rect _viewport;

        Vector4d<srGLfloat> _clearColor;
    };
}

#endif // GLSR_DRAWING_SERVICE_H