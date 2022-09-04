#pragma once
#include <QTime>

class Consumption
{
public:
    Consumption();
    bool isValid() const;
    QTime time() const;
    qreal kW() const;
    void setKWh(qreal value);
    void setKW(qreal value);
    void setTime(QTime newTime);
private:
    QTime m_time = QTime(0,0);
    qreal m_kWh = 0;
    qreal m_kW = 0;
};
