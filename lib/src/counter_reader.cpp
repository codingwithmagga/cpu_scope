#include "counter_reader.hpp"

#include <cstdint>

using namespace cpuscope;

bool CounterReader::start()
{
    return true;
}

bool CounterReader::stop()
{
    return true;
}

std::optional<uint64_t> CounterReader::get_metrics() const
{
    return 123456789;  // Placeholder value for demonstration
}