#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <utility>

#include "perf_event.hpp"

MATCHER_P(PerfAttrEq, expected, "")
{
    return arg && arg->type == expected.type && arg->size == expected.size && arg->config == expected.config;
}

struct FakeSysCalls : PerfEvent::ISysCalls
{
    MOCK_METHOD(int, perf_event_open, (const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags), (override));

    MOCK_METHOD(int, close, (int), (override));
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

    FakeSysCalls m_sysCalls;
    perf_event_attr m_attr;
};

TEST_F(PerfEventTest, OpenPerfEventCPUScope)
{
    PerfEvent::Config config;
    config.scope = PerfEvent::Scope::CPU;
    config.cpu = 2;
    config.pid = 1000;  // Automatically set to -1 for CPU scope
    config.event = PerfEvent::Event::CPU_CYCLES;
    int fd = 42;

    EXPECT_CALL(m_sysCalls, perf_event_open(PerfAttrEq(m_attr), -1, 2, -1, 0)).WillOnce(testing::Return(fd));
    EXPECT_CALL(m_sysCalls, close(fd)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, m_sysCalls);

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
    int fd = 44;

    EXPECT_CALL(m_sysCalls, perf_event_open(PerfAttrEq(m_attr), 1234, -1, -1, 0)).WillOnce(testing::Return(fd));
    EXPECT_CALL(m_sysCalls, close(fd)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, m_sysCalls);

    EXPECT_TRUE(res);
}

TEST_F(PerfEventTest, OpenPerfEventCacheMisses)
{
    PerfEvent::Config config;
    config.pid = 1234;
    config.scope = PerfEvent::Scope::Process;
    config.event = PerfEvent::Event::HW_Cache_Misses;
    m_attr.config = PERF_COUNT_HW_CACHE_MISSES;
    int fd = 42;

    EXPECT_CALL(m_sysCalls, perf_event_open(PerfAttrEq(m_attr), 1234, -1, -1, 0)).WillOnce(testing::Return(fd));
    EXPECT_CALL(m_sysCalls, close(fd)).WillOnce(testing::Return(0));

    auto res = PerfEvent::open(config, m_sysCalls);

    EXPECT_TRUE(res);
}

TEST_F(PerfEventTest, OpenPerfEventWithInvalidCPUConfig)
{
    PerfEvent::Config config;
    config.cpu = -1;
    config.scope = PerfEvent::Scope::CPU;

    auto res = PerfEvent::open(config, m_sysCalls);

    EXPECT_FALSE(res);
}

TEST_F(PerfEventTest, OpenPerfEventWithInvalidProcessConfig)
{
    PerfEvent::Config config;
    config.pid = -1;
    config.scope = PerfEvent::Scope::Process;

    auto res = PerfEvent::open(config, m_sysCalls);

    EXPECT_FALSE(res);
}

TEST_F(PerfEventTest, MoveCtorTransfersOwnership)
{
    PerfEvent::Config config;
    config.scope = PerfEvent::Scope::CPU;
    config.cpu = 2;

    EXPECT_CALL(m_sysCalls, perf_event_open(testing::_, -1, 2, -1, 0)).WillOnce(testing::Return(42));

    EXPECT_CALL(m_sysCalls, close(42)).Times(1);

    {
        auto original = PerfEvent::open(config, m_sysCalls);

        ASSERT_TRUE(original.has_value());

        PerfEvent moved(std::move(*original));
    }
}

TEST_F(PerfEventTest, MoveAssignmentTransfersOwnership)
{
    PerfEvent::Config config1;
    config1.scope = PerfEvent::Scope::CPU;
    config1.cpu = 1;

    PerfEvent::Config config2;
    config2.scope = PerfEvent::Scope::CPU;
    config2.cpu = 2;

    EXPECT_CALL(m_sysCalls, perf_event_open(testing::_, -1, 1, -1, 0)).WillOnce(testing::Return(10));

    EXPECT_CALL(m_sysCalls, perf_event_open(testing::_, -1, 2, -1, 0)).WillOnce(testing::Return(42));

    EXPECT_CALL(m_sysCalls, close(10)).Times(1);
    EXPECT_CALL(m_sysCalls, close(42)).Times(1);

    {
        auto lhs = PerfEvent::open(config1, m_sysCalls);
        auto rhs = PerfEvent::open(config2, m_sysCalls);

        ASSERT_TRUE(lhs.has_value());
        ASSERT_TRUE(rhs.has_value());

        *lhs = std::move(*rhs);
    }
}