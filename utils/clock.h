#pragma once

#include <QObject>
#include <QTime>

class Clock : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentTime READ currentTimeString NOTIFY secondChanged)
public:
    explicit Clock(QObject *parent = nullptr);
    QString currentTimeString() const {return m_currentTimeString;}
    QTime currentTime() const {return m_currentTime;}
    QTime nextEvenHour() const {return m_nextEvenHour;}
signals:
    void secondChanged();
    void minuteChanged();
    void hourChanged();
protected:
    void timerEvent(QTimerEvent *event) override;
private:
    QString m_currentTimeString;
    QTime m_currentTime;
    QTime m_nextEvenHour;
    int m_currentHour = -1;
    int m_currentMinute = -1;
};
