#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "counter_reader.hpp"

using namespace cpuscope;

TEST(CounterReaderTest, Create)
{
    CounterReader cnt_reader;
    EXPECT_TRUE(cnt_reader.start());
    EXPECT_TRUE(cnt_reader.stop());

    auto value = cnt_reader.get_metrics();

    EXPECT_TRUE(value.has_value());
}