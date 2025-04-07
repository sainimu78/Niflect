#include "Niflect/NiflectMethod.h"
#include "Niflect/Util/StringUtil.h"

namespace Niflect
{
	HashInt ComputeSignatureHash(const Niflect::CString& str)
	{
		return NiflectUtil::StableHash(str);
	}
}