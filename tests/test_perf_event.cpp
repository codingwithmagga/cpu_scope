#include <gtest/gtest.h>

#include "perf_event.hpp"

TEST(PerfEvent, OpenPerfEvent)
{
    PerfEvent::Config config;
    auto res = PerfEvent::open(config);
    EXPECT_TRUE(res);
}