#ifndef GLSR_SPIRVM_EXTENSIONS_H
#define GLSR_SPIRVM_EXTENSIONS_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <spvm/state.h>

#include "../Models/OutputValue.h"

namespace glsr::Services::Shader::SpirVm
{
    class SpirVmExtensions
    {
    public:
        static spvm_result_t findInputByLocation(spvm_state_t state, uint32_t location);

        static bool isUniformExists(spvm_state_t state, const std::string &name);

        static bool setInputByLocation(spvm_state_t state, uint32_t location, spvm_value_type componentType, const void *value, size_t componentCount);

        static bool setInputByName(spvm_state_t state, const std::string &name, spvm_value_type componentType, const void *value);

        static std::vector<Models::OutputValue> collectShaderOutputs(spvm_state_t state);

        static spvm_result_t findUniformByName(spvm_state_t state, const std::string &name);

        static bool setUniformUintValueByName(spvm_state_t state, const std::string &name, uint32_t value);

        static bool setUniformVectorFloatByName(spvm_state_t state, const std::string &name, const float *value);

        static bool setUniformMatrix4fvByName(spvm_state_t state, const std::string &name, const float *value);
    private:
        static void _collectBuiltinOutput(spvm_state_t state, SpvBuiltIn builtin, const std::string &name, std::vector<Models::OutputValue> &outputs);
    };
}

#endif // GLSR_SPIRVM_EXTENSIONS_H