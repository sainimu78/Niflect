#include "NiflectGen/Util/WriterUtil.h"
#include "Niflect/Util/StringUtil.h"

namespace NiflectGen
{
	Niflect::CString ConvertHashToString(const Niflect::HashInt& hashCode)
	{
		return NiflectUtil::FormatString("%lluu", hashCode);
	}
}