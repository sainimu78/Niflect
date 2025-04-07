#include "Niflect/NiflectType.h"
#include "Niflect/Util/StringUtil.h"

namespace Niflect
{
	HashInt ComputeTypeHash(const Niflect::CString& str)
	{
		return NiflectUtil::StableHash(str);
	}
}