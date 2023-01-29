#pragma once
#include <QObject>

class DebugInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString debugString READ debugString NOTIFY debugStringChanged)
public:
    static DebugInfo* instance();
    static void destroyInstance();
    QString debugString() const;
    void print(const QString &string);
signals:
    void debugStringChanged();
private:
    explicit DebugInfo();
    static DebugInfo* m_instance;
    QStringList m_debugStrings;
};
