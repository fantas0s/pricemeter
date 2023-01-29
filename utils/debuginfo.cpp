#include "debuginfo.h"

DebugInfo* DebugInfo::m_instance = nullptr;

DebugInfo *DebugInfo::instance()
{
    if (m_instance == nullptr)
        m_instance = new DebugInfo();
    return m_instance;
}

QString DebugInfo::debugString() const
{
    return m_debugString;
}

void DebugInfo::setDebugString(const QString &string)
{
    if (m_debugString != string)
    {
        m_debugString = string;
        emit debugStringChanged();
    }
}

DebugInfo::DebugInfo()
    : QObject{nullptr}
{
}
