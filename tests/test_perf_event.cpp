#include <gtest/gtest.h>

#include "perf_event.hpp"

TEST(PerfEvent, OpenPerfEvent)
{
    auto res = PerfEvent::open();
    EXPECT_TRUE(res);
}