#pragma once
#include <QObject>

class DebugInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString debugString READ debugString WRITE setDebugString NOTIFY debugStringChanged)
public:
    static DebugInfo* instance();
    QString debugString() const;
    void setDebugString(const QString &string);
signals:
    void debugStringChanged();
private:
    explicit DebugInfo();
    static DebugInfo* m_instance;
    QString m_debugString;
};
