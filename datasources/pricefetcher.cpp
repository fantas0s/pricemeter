#include "pricefetcher.h"

PriceFetcher::PriceFetcher(Clock* clock)
    : QObject{nullptr}
    , m_clock{clock}
{
}
