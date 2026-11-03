#ifndef GLSR_SOURCES_SERVICES_CONFIGURATION_CONFIGURATIONSERVICE_H
#define GLSR_SOURCES_SERVICES_CONFIGURATION_CONFIGURATIONSERVICE_H

#include <cstdint>
#include <map>

#include "IConfigurationService.h"

namespace glsr::Services::Configuration
{
    class ConfigurationService : public IConfigurationService
    {
    public:
        ~ConfigurationService() override = default;

        void enable(srGLenum cap) override;

        void disable(srGLenum cap) override;

        void getBooleanv(srGLenum pname, srGLboolean *data) const override;

        void blendFunc(srGLenum sfactor, srGLenum dfactor) override;

        void cullFace(srGLenum mode) override;

        void getIntegerv(srGLenum pname, srGLint *data) const override;
    private:
        std::map<srGLenum, bool> _capabilities;
        std::map<srGLenum, srGLenum> _blendFuncs;
        srGLenum _cullFaceMode;
    };
}

#endif // GLSR_SOURCES_SERVICES_CONFIGURATION_CONFIGURATIONSERVICE_H