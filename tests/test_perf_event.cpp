#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "perf_event.hpp"

struct FakePerfSysCall : PerfEvent::IPerfSysCall
{
    MOCK_METHOD(int, perf_event_open, (const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags), (override));
};

TEST(PerfEvent, OpenPerfEventDefaultConfig)
{
    PerfEvent::Config config;
    FakePerfSysCall perfSysCall;

    EXPECT_CALL(perfSysCall, perf_event_open(nullptr, 0, -1, -1, 0)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_TRUE(res);
}

TEST(PerfEvent, OpenPerfEventCustomConfig)
{
    PerfEvent::Config config;
    config.pid = 1234;
    config.cpu = 2;
    FakePerfSysCall perfSysCall;

    EXPECT_CALL(perfSysCall, perf_event_open(nullptr, 1234, 2, -1, 0)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_TRUE(res);
}

TEST(PerfEvent, OpenPerfEventWithInvalidPidCPUConfig)
{
    PerfEvent::Config config;
    config.pid = -1;
    config.cpu = -1;
    FakePerfSysCall perfSysCall;

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_FALSE(res);
}
