#include "NiflectGen/Util/ParserUtil.h"
#include "NiflectGen/Base/NiflectGenDefinition.h"

namespace NiflectGen
{
    void CParserOption::InitDefault()
    {
        m_vecArg.push_back("-x");
        m_vecArg.push_back("c++");
        m_vecArg.push_back("-std=c++14");
        m_vecArg.push_back("-DNDEBUG");
        m_vecArg.push_back("-D__clang__");
        m_vecArg.push_back("-w");
        m_vecArg.push_back("-MG");
        m_vecArg.push_back("-M");
        m_vecArg.push_back("-ferror-limit=0");
        m_vecArg.push_back("-o clangLog.txt");
        m_vecArg.push_back(CONCAT_HARDCODED_STRINGS_3("-D", NIFLECT_GEN_TIME_MACRO, "="));
        this->UpdateArgV();
    }
}