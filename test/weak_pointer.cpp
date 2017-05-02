#include <gtest/gtest.h>
#include "definitions.h"

TEST(WeakPointerTest, IncrementCounter) {
  {
    auto s = landfill::StrongPointer<S>::Make();
    s->ptrs.push_back(landfill::WeakPointer<S>{s.Get(), new S{}});
    ASSERT_EQ(S::cnt, 2);
    landfill::GC();
    ASSERT_EQ(S::cnt, 2);
  }
  landfill::GC();
  ASSERT_EQ(S::cnt, 0);
}

TEST(WeakPointerTest, IncrementCounterMultiple) {
  {
    auto s = landfill::StrongPointer<S>::Make();
    for (int i = 0; i < 100; ++i) {
      s->ptrs.push_back(landfill::WeakPointer<S>{s.Get(), new S{}});
    }
    ASSERT_EQ(S::cnt, 101);
    landfill::GC();
    ASSERT_EQ(S::cnt, 101);
  }
  landfill::GC();
  ASSERT_EQ(S::cnt, 0);
}
