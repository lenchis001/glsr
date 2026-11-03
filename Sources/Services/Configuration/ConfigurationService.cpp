#include "ConfigurationService.h"

#include <cassert>

using namespace glsr::Services::Configuration;

void ConfigurationService::enable(srGLenum cap)
{
    _capabilities[cap] = true;
}

void ConfigurationService::disable(srGLenum cap)
{
    _capabilities[cap] = false;
}

void ConfigurationService::getBooleanv(srGLenum pname, srGLboolean *data) const
{
    auto it = _capabilities.find(pname);
    *data = it != _capabilities.end() && it->second;
}

void ConfigurationService::blendFunc(srGLenum sfactor, srGLenum dfactor)
{
    assert(sfactor == srGL_SRC_ALPHA && dfactor == srGL_ONE_MINUS_SRC_ALPHA && "Only GL_SRC_ALPHA-GL_ONE_MINUS_SRC_ALPHA is allowed!");
    _blendFuncs[sfactor] = dfactor;
}

void ConfigurationService::cullFace(srGLenum mode)
{
    _cullFaceMode = mode;
}

void ConfigurationService::getIntegerv(srGLenum pname, srGLint *data) const
{
    if (pname == srGL_CULL_FACE)
    {
        *data = static_cast<srGLint>(_cullFaceMode);
    }
    else
    {
        assert(false && "Unsupported pname for getIntegerv");
    }
}
