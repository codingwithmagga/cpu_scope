#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>

#include "perf_event.hpp"

MATCHER_P(PerfAttrEq, expected, "")
{
    return arg && arg->type == expected.type && arg->size == expected.size && arg->config == expected.config;
}

struct FakePerfSysCall : PerfEvent::IPerfSysCall
{
    MOCK_METHOD(int, perf_event_open, (const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags), (override));
};

class PerfEventTest : public testing::Test
{
protected:
    void SetUp() override
    {
        std::memset(&m_attr, 0, sizeof(m_attr));
        m_attr.type = PERF_TYPE_HARDWARE;
        m_attr.size = sizeof(m_attr);
        m_attr.config = PERF_COUNT_HW_CPU_CYCLES;
    }

    FakePerfSysCall perfSysCall;
    perf_event_attr m_attr;
};

TEST_F(PerfEventTest, OpenPerfEventCPUScope)
{
    PerfEvent::Config config;
    config.scope = PerfEvent::Scope::CPU;
    config.cpu = 2;
    config.pid = 1000;  // Automatically set to -1 for CPU scope
    config.event = PerfEvent::Event::CPU_CYCLES;

    EXPECT_CALL(perfSysCall, perf_event_open(PerfAttrEq(m_attr), -1, 2, -1, 0)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_TRUE(res);
}

TEST_F(PerfEventTest, OpenPerfEventProcessScope)
{
    PerfEvent::Config config;
    config.cpu = 5;  // Automatically set to -1 for PID scope
    config.pid = 1234;
    config.scope = PerfEvent::Scope::Process;
    config.event = PerfEvent::Event::HW_Instructions;
    m_attr.config = PERF_COUNT_HW_INSTRUCTIONS;

    EXPECT_CALL(perfSysCall, perf_event_open(PerfAttrEq(m_attr), 1234, -1, -1, 0)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_TRUE(res);
}

TEST_F(PerfEventTest, OpenPerfEventCacheMisses)
{
    PerfEvent::Config config;
    config.pid = 1234;
    config.scope = PerfEvent::Scope::Process;
    config.event = PerfEvent::Event::HW_Cache_Misses;
    m_attr.config = PERF_COUNT_HW_CACHE_MISSES;

    EXPECT_CALL(perfSysCall, perf_event_open(PerfAttrEq(m_attr), 1234, -1, -1, 0)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_TRUE(res);
}

TEST_F(PerfEventTest, OpenPerfEventWithInvalidCPUConfig)
{
    PerfEvent::Config config;
    config.cpu = -1;
    config.scope = PerfEvent::Scope::CPU;

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_FALSE(res);
}

TEST_F(PerfEventTest, OpenPerfEventWithInvalidProcessConfig)
{
    PerfEvent::Config config;
    config.pid = -1;
    config.scope = PerfEvent::Scope::Process;

    auto res = PerfEvent::open(config, perfSysCall);

    EXPECT_FALSE(res);
}
