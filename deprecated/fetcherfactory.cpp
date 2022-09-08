#include "datasources/fetcherfactory.h"
#include "htmlfetcher.h"

PriceFetcher *FetcherFactory::getFetcherImplementation(Clock* clock)
{
    return new HtmlFetcher(clock);
}
