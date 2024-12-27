#pragma once
#include "Niflect/Base/NiflectBase.h"
#include "mustache/mustache.hpp"

namespace NiflectMustache
{
	using CMustache = kainjow::mustache::basic_mustache<Niflect::CString, Niflect::CMemory>;
	using CData = kainjow::mustache::basic_data<CMustache::string_type, CMustache::CMemory>;
	using CObject = kainjow::mustache::basic_object<CMustache::string_type, CMustache::CMemory>;
	using CList = kainjow::mustache::basic_list<CMustache::string_type, CMustache::CMemory>;
	using CPartial = kainjow::mustache::basic_partial<CMustache::string_type>;
	using CRenderer = kainjow::mustache::basic_renderer<CMustache::string_type>;
	using CLambda = kainjow::mustache::basic_lambda<CMustache::string_type, CMustache::CMemory>;
	using CLambda2 = kainjow::mustache::basic_lambda2<CMustache::string_type, CMustache::CMemory>;
	using CLambda_t = kainjow::mustache::basic_lambda_t<CMustache::string_type, CMustache::CMemory>;

	//using mustachew = kainjow::mustache::basic_mustache<Niflect::CString>;
	//using dataw = kainjow::mustache::basic_data<Niflect::CString>;
}