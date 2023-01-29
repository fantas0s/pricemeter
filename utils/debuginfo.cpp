#include "debuginfo.h"

DebugInfo* DebugInfo::m_instance = nullptr;

static const int s_rowMax = 10;

DebugInfo *DebugInfo::instance()
{
    if (m_instance == nullptr)
        m_instance = new DebugInfo();
    return m_instance;
}

void DebugInfo::destroyInstance()
{
    delete m_instance;
    m_instance = nullptr;
}

QString DebugInfo::debugString() const
{
    return m_debugStrings.join(QChar('\n'));
}

void DebugInfo::print(const QString &string)
{
    while (m_debugStrings.size() >= s_rowMax)
    {
        m_debugStrings.removeFirst();
    }
    m_debugStrings.append(string);
    emit debugStringChanged();
}

DebugInfo::DebugInfo()
    : QObject{nullptr}
{
}
