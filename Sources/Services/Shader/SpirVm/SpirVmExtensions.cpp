#include "SpirVmExtensions.h"

#include "srtypes.h"
#include "srglext.h"

using namespace glsr::Services::Shader::SpirVm;
using namespace glsr::Services::Shader::Models;

spvm_result_t SpirVmExtensions::findInputByLocation(spvm_state_t state, uint32_t location)
{
	if (!state || !state->owner)
		return NULL;

	for (spvm_word i = 0; i < state->owner->bound; i++)
	{
		spvm_result_t result = &state->results[i];

		if (result->storage_class != SpvStorageClassInput)
			continue;

		for (spvm_word d = 0; d < result->decoration_count; d++)
		{
			if (result->decorations[d].type == SpvDecorationLocation &&
				static_cast<uint32_t>(result->decorations[d].literal1) == location)
				return result;
		}
	}

	return NULL;
}

bool SpirVmExtensions::isUniformExists(spvm_state_t state, const std::string &name)
{
	auto uniformResult = findUniformByName(state, name);

	return uniformResult != nullptr;
}

bool SpirVmExtensions::setInputByLocation(
	spvm_state_t state,
	uint32_t location,
	spvm_value_type componentType,
	const void *value,
	size_t componentCount)
{
	if (!state || !value)
		return false;

	spvm_result_t result = findInputByLocation(state, location);

	if (!result || !result->members)
		return false;

	if (static_cast<size_t>(result->member_count) != componentCount)
		return false;

	switch (componentType)
	{
	case spvm_value_type_float:
		spvm_member_set_value_f(result->members, componentCount, const_cast<float *>(static_cast<const float *>(value)));
		return true;
	case spvm_value_type_int:
		spvm_member_set_value_i(result->members, componentCount, const_cast<int *>(static_cast<const int *>(value)));
		return true;
	default:
		return false;
	}
}

bool SpirVmExtensions::setInputByName(
	spvm_state_t state,
	const std::string &name,
	spvm_value_type componentType,
	const void *value)
{
	if (!state || !value)
		return false;

	for (spvm_word i = 0; i < state->owner->bound; i++)
	{
		spvm_result_t result = &state->results[i];

		if (result->storage_class != SpvStorageClassInput)
			continue;

		if (result->name && name == result->name)
		{
			if (!result->members)
				return false;

			switch (componentType)
			{
			case spvm_value_type_float:
				spvm_member_set_value_f(result->members, result->member_count, const_cast<float *>(static_cast<const float *>(value)));
				return true;
			case spvm_value_type_int:
				spvm_member_set_value_i(result->members, result->member_count, const_cast<int *>(static_cast<const int *>(value)));
				return true;
			case spvm_value_type_array:
				spvm_member_set_value_f(result->members, result->member_count, const_cast<float *>(static_cast<const float *>(value)));
				return true;
			case spvm_value_type_vector:
				spvm_member_set_value_f(result->members, result->member_count, const_cast<float *>(static_cast<const float *>(value)));
				return true;
			case spvm_value_type_matrix:
				spvm_member_set_value_f(result->members, result->member_count, const_cast<float *>(static_cast<const float *>(value)));
				return true;
			default:
				return false;
			}
		}
	}

	return false;
}

std::vector<OutputValue> SpirVmExtensions::collectShaderOutputs(spvm_state_t state)
{
	std::vector<OutputValue> outputs;

	for (spvm_word i = 0; i < state->owner->bound; i++)
	{
		spvm_result_t result = &state->results[i];

		if (result->storage_class != SpvStorageClassOutput || !result->members)
			continue;

		spvm_result_t typeInfo = spvm_state_get_type_info(state->results, &state->results[result->pointer]);

		OutputValue outputValue;
		outputValue.name = result->name ? result->name : "";

		if (typeInfo->value_type == spvm_value_type_float && result->member_count == 1)
		{
			outputValue.type = srGL_FLOAT;
			outputValue.value.floatValue = result->members[0].value.f;
		}
		else if (typeInfo->value_type == spvm_value_type_int && result->member_count == 1)
		{
			outputValue.type = srGL_INT;
			outputValue.value.intValue = result->members[0].value.s;
		}
		else if (typeInfo->value_type == spvm_value_type_vector && result->member_count == 2 &&
				 spvm_state_get_type_info(state->results, &state->results[typeInfo->pointer])->value_type == spvm_value_type_float)
		{
			outputValue.type = srGL_FLOAT_VEC2;
			outputValue.value.floatVector2dValue.x = result->members[0].value.f;
			outputValue.value.floatVector2dValue.y = result->members[1].value.f;
		}
		else if (typeInfo->value_type == spvm_value_type_vector && result->member_count == 3 &&
				 spvm_state_get_type_info(state->results, &state->results[typeInfo->pointer])->value_type == spvm_value_type_float)
		{
			outputValue.type = srGL_FLOAT_VEC3;
			outputValue.value.floatVector3dValue.x = result->members[0].value.f;
			outputValue.value.floatVector3dValue.y = result->members[1].value.f;
			outputValue.value.floatVector3dValue.z = result->members[2].value.f;
		}
		else if (typeInfo->value_type == spvm_value_type_vector && result->member_count == 4 &&
				 spvm_state_get_type_info(state->results, &state->results[typeInfo->pointer])->value_type == spvm_value_type_float)
		{
			outputValue.type = srGL_FLOAT_VEC4;
			outputValue.value.floatVector4dValue.x = result->members[0].value.f;
			outputValue.value.floatVector4dValue.y = result->members[1].value.f;
			outputValue.value.floatVector4dValue.z = result->members[2].value.f;
			outputValue.value.floatVector4dValue.w = result->members[3].value.f;
		}
		else
		{
			continue;
		}

		outputs.push_back(std::move(outputValue));
	}

	// Built-in outputs (e.g. gl_Position) aren't decorated with a Location, so they
	// aren't picked up by the loop above and need to be looked up explicitly.
	_collectBuiltinOutput(state, SpvBuiltInPosition, "gl_Position", outputs);
	_collectBuiltinOutput(state, SpvBuiltInPointSize, "gl_PointSize", outputs);
	_collectBuiltinOutput(state, SpvBuiltInFragDepth, "gl_FragDepth", outputs);

	return outputs;
}

spvm_result_t SpirVmExtensions::findUniformByName(spvm_state_t state, const std::string &name)
{
	if (!state || !state->owner)
		return NULL;

	for (spvm_word i = 0; i < state->owner->bound; i++)
	{
		spvm_result_t result = &state->results[i];

		if (result->storage_class != SpvStorageClassUniformConstant)
			continue;

		if (result->name && name == result->name)
			return result;
	}

	return NULL;
}

bool SpirVmExtensions::setUniformUintValueByName(spvm_state_t state, const std::string &name, uint32_t value)
{
	spvm_result_t result = findUniformByName(state, name);
	if (!result || !result->decorations || !result->decoration_count)
		return false;

	for (spvm_word d = 0; d < result->decoration_count; d++)
	{
		if (result->decorations[d].type == SpvDecorationBinding)
		{
			result->decorations[d].literal1 = value;
			break;
		}
	}
	return true;
}

bool SpirVmExtensions::setUniformVectorFloatByName(spvm_state_t state, const std::string &name, const float *value)
{
	spvm_result_t result = findUniformByName(state, name);

	if (!result || !result->members || static_cast<size_t>(result->member_count) < 1)
		return false;

	for (spvm_word i = 0; i < result->member_count; ++i)
	{
		result->members[i].value.f = value[i];
	}

	return true;
}

bool SpirVmExtensions::setUniformMatrix4fvByName(spvm_state_t state, const std::string &name, const float *value)
{
    spvm_result_t result = findUniformByName(state, name);

    if (!result || !result->members || result->member_count < 1)
        return false;

    for (spvm_word i = 0; i < result->member_count; ++i)
    {
        auto column = &result->members[i];

        if (!column->members || column->member_count < 1)
            return false;

        for (spvm_word j = 0; j < column->member_count; ++j)
        {
            column->members[j].value.f = value[i * 4 + j];
        }
    }

    return true;
}

void SpirVmExtensions::_collectBuiltinOutput(
	spvm_state_t state,
	SpvBuiltIn builtin,
	const std::string &name,
	std::vector<OutputValue> &outputs)
{
	spvm_word memberCount = 0;
	spvm_member_t members = spvm_state_get_builtin(state, builtin, &memberCount);

	if (!members || memberCount == 0)
		return;

	OutputValue outputValue;
	outputValue.name = name;

	switch (memberCount)
	{
	case 1:
		outputValue.type = srGL_FLOAT;
		outputValue.value.floatValue = members[0].value.f;
		break;
	case 3:
		outputValue.type = srGL_FLOAT_VEC3;
		outputValue.value.floatVector3dValue.x = members[0].value.f;
		outputValue.value.floatVector3dValue.y = members[1].value.f;
		outputValue.value.floatVector3dValue.z = members[2].value.f;
		break;
	case 4:
		outputValue.type = srGL_FLOAT_VEC4;
		outputValue.value.floatVector4dValue.x = members[0].value.f;
		outputValue.value.floatVector4dValue.y = members[1].value.f;
		outputValue.value.floatVector4dValue.z = members[2].value.f;
		outputValue.value.floatVector4dValue.w = members[3].value.f;
		break;
	default:
		return;
	}

	outputs.push_back(std::move(outputValue));
}