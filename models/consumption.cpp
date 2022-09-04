#include "consumption.h"

Consumption::Consumption()
{}

bool Consumption::isValid() const
{
    return kW() != 0;
}

QTime Consumption::time() const
{
    return m_time;
}

qreal Consumption::kW() const
{
    if (m_kW > 0)
    {
        return m_kW;
    }
    else
    {
        const int msecs = m_time.msecsSinceStartOfDay();
        if (msecs > 0) {
            qreal hours = ((qreal)msecs) / 3600000.0;
            return m_kWh / hours;
        } else {
            return 0;
        }
    }
}

void Consumption::setKWh(qreal value)
{
    m_kWh = value;
}

void Consumption::setKW(qreal value)
{
    m_kW = value;
}

void Consumption::setTime(QTime newTime)
{
    m_time = newTime;
}
