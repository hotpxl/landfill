#include <gtest/gtest.h>
#include "definitions.h"

TEST(StrongPointerTest, IncrementCounter) {
  S* ptr = new S{};
  landfill::StrongPointer<S> s{ptr};
  ASSERT_EQ(S::cnt, 1);
  delete ptr;
}

TEST(StrongPointerTest, DecrementCounter) {
  {
    auto s = landfill::StrongPointer<S>::Make();
    ASSERT_EQ(S::cnt, 1);
  }
  landfill::GC();
  ASSERT_EQ(S::cnt, 0);
}
