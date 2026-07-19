#pragma once

#include <gtest/gtest.h>

class StreamBufferTest : public ::testing::Test
{
public:
    static void SetUpTestSuite();
    static void TearDownTestSuite();
};


// EXECUTION ORDER

// SetUpTestSuite()
//   Test Add
//   Test Subtract
//   ...
// TearDownTestSuite()