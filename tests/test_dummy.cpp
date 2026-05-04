#include <gtest/gtest.h>

#include "version.hpp"

TEST(DummyTest, BasicAssertion)
{
    EXPECT_EQ(1 + 1, 2);
}

TEST(DummyTest, VersionCheck)
{
    EXPECT_EQ(cpuscope::get_version(), "0.1.0");
}