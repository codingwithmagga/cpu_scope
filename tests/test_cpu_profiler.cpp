#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "cpu_profiler.hpp"
#include "perf_event.hpp"

using namespace cpuscope;

struct FakeSysCalls : PerfEvent::ISysCalls
{
    MOCK_METHOD(int, perf_event_open, (const perf_event_attr* attr, pid_t pid, int cpu, int group_fd, unsigned long flags), (override));

    MOCK_METHOD(int, close, (int), (override));

    MOCK_METHOD(ssize_t, read, (int file_descriptor, void* buf, size_t count), ());
};

class CpuProfilerTest : public testing::Test
{
protected:
    FakeSysCalls m_sysCalls;
};

TEST_F(CpuProfilerTest, ReadMetrics)
{
    CpuProfiler cpu_profiler(m_sysCalls);
    uint64_t counterValue = 123456789;
    const int file_descriptor = 42;

    EXPECT_CALL(m_sysCalls, read(file_descriptor, ::testing::_, sizeof(counterValue)))
        .WillOnce(testing::Invoke(
            [&](int, void* buf, size_t)
            {
                *static_cast<uint64_t*>(buf) = counterValue;
                return static_cast<ssize_t>(sizeof(counterValue));
            }));

    EXPECT_TRUE(cpu_profiler.start());
    EXPECT_TRUE(cpu_profiler.stop());

    auto value = cpu_profiler.get_metrics();

    EXPECT_TRUE(value.has_value());
    EXPECT_EQ(value.value(), counterValue);
}