#pragma once
#include "Niflect/NiflectAccessorSetting.h"
#include "Niflect/NiflectAccessor.h"
#include "Niflect/Base/Array.h"

namespace TestAS
{
	class CPlaceholderValueAccessor : public Niflect::CAccessor
	{
	};
	template <typename TInstance>
	class TPlaceholderContainerAccessor : public Niflect::CAccessor
	{
	};

	using namespace NiflectAccessorSetting;

	NIF_AS_A() TSetting<CPlaceholderValueAccessor, float>;
	template <typename TInstance, typename T>
	NIF_AS_A() TSetting<TPlaceholderContainerAccessor<TInstance>, Niflect::TArray<T> >;
}