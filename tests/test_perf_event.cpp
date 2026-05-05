#include <gtest/gtest.h>

#include "perf_event.hpp"

TEST(PerfEvent, OpenPerfEvent)
{
    PerfEvent::Config config;

    auto res = PerfEvent::open(config);

    EXPECT_TRUE(res);
}

TEST(PerfEvent, OpenPerfEventWithInvalidConfig)
{
    PerfEvent::Config config;
    config.pid = -1;
    config.cpu = -1;

    auto res = PerfEvent::open(config);

    EXPECT_FALSE(res);
}