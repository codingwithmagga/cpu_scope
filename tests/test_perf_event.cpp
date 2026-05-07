#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "perf_event.hpp"

struct FakePerfSysCall : PerfEvent::IPerfSysCall
{
    MOCK_METHOD(int, perf_event_open, (const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags), (override));
};

TEST(PerfEvent, OpenPerfEventCPUScope)
{
    PerfEvent::Config config;
    config.scope = PerfEvent::Scope::CPU;
    config.cpu = 2;
    config.pid = 1000;  // Automatically set to -1 for CPU scope
    FakePerfSysCall perfSysCall;

    EXPECT_CALL(perfSysCall, perf_event_open(nullptr, -1, 2, -1, 0)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_TRUE(res);
}

TEST(PerfEvent, OpenPerfEventCustomConfig)
{
    PerfEvent::Config config;
    config.cpu = 5;  // Automatically set to -1 for PID scope
    config.pid = 1234;
    config.scope = PerfEvent::Scope::Process;
    FakePerfSysCall perfSysCall;

    EXPECT_CALL(perfSysCall, perf_event_open(nullptr, 1234, -1, -1, 0)).WillOnce(testing::Return(0));

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
