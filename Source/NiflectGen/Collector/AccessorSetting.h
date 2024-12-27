#pragma once
#include "Niflect/NiflectCommon.h"
#include "Niflect/Util/ConcatHardCodedStrings.h"

#define CODETAG_PREFIX_ACCESSOR_SETTING "__NiflectGen_AccessorSetting"

namespace NiflectGen
{
	enum class EAccessorSettingTagType
	{
		A,
		B,
		C,
		D,
		E,
		Count,
	};
	constexpr const char* AccessorSetingTagNames[static_cast<uint32>(EAccessorSettingTagType::Count)] = {
		CONCAT_HARDCODED_STRINGS_2(CODETAG_PREFIX_ACCESSOR_SETTING, "_A"),
		CONCAT_HARDCODED_STRINGS_2(CODETAG_PREFIX_ACCESSOR_SETTING, "_B"),
		CONCAT_HARDCODED_STRINGS_2(CODETAG_PREFIX_ACCESSOR_SETTING, "_C"),
		CONCAT_HARDCODED_STRINGS_2(CODETAG_PREFIX_ACCESSOR_SETTING, "_D"),
		CONCAT_HARDCODED_STRINGS_2(CODETAG_PREFIX_ACCESSOR_SETTING, "_E"),
	};
	static uint32 GetAccessorSettingTypesCount()
	{
		return static_cast<uint32>(EAccessorSettingTagType::Count);
	}
}