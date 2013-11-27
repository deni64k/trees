#pragma once

#include <gtest/gtest.h>

class Test
: public ::testing::Test {
protected:
  Test()
  {
    // Class setup
  }

  virtual ~Test() {
    // Class teardown
  }

  virtual void SetUp() {
    // Setup before method
  }

  virtual void TearDown() {
    // Teardown before method
  }
};
