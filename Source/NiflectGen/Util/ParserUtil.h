#pragma once
#include "Niflect/NiflectBase.h"

namespace NiflectGen
{
    class CParserOption
    {
    public:
        void InitDefault();
        void AddIncludePath(const Niflect::CString& path)
        {
            m_vecArg.push_back("-I" + path);
            this->UpdateArgV();
        }
        void AddIncludePaths(const Niflect::TArrayNif<Niflect::CString>& vecPath)
        {
            for (auto& it : vecPath)
                m_vecArg.push_back("-I" + it);
            this->UpdateArgV();
        }
        int GetArgC() const
        {
            return static_cast<uint32>(m_vecArgV.size());
        }
        const char* const* GetArgV() const
        {
            return m_vecArgV.data();
        }


    private:
        void UpdateArgV()
        {
            m_vecArgV.clear();
            for (auto& it : m_vecArg)
                m_vecArgV.push_back(&it[0]);
        }

    public:
        Niflect::TArrayNif<Niflect::CString> m_vecArg;
        Niflect::TArrayNif<const char*> m_vecArgV;
    };
}