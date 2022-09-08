#pragma once

class Clock;
class PriceFetcher;

class FetcherFactory
{
public:
    static PriceFetcher* getFetcherImplementation(Clock* clock);
private:
    FetcherFactory() = delete;
};
