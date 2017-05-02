#include <gtest/gtest.h>
#include <chrono>
#include <random>
#include "definitions.h"

TEST(Completeness, CircularDependency) {
  {
    auto a = landfill::StrongPointer<S>::Make();
    auto b = landfill::StrongPointer<S>::Make();
    a->ptrs.push_back(landfill::WeakPointer<S>{a.Get(), b.Get()});
    b->ptrs.push_back(landfill::WeakPointer<S>{b.Get(), a.Get()});
    ASSERT_EQ(S::cnt, 2);
    landfill::GC();
    ASSERT_EQ(S::cnt, 2);
  }
  landfill::GC();
  ASSERT_EQ(S::cnt, 0);
}

TEST(Completeness, ComplexCircularDependency) {
  constexpr int const kTotal = 2048;
  std::vector<landfill::StrongPointer<S>> ptrs;
  for (int i = 0; i < kTotal; ++i) {
    ptrs.push_back(landfill::StrongPointer<S>::Make());
  }
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<> dis{0, 2047};
  for (int i = 0; i < kTotal; ++i) {
    for (int j = 0; j < 1024; ++j) {
      ptrs[i]->ptrs.push_back(
          landfill::WeakPointer<S>(ptrs[i].Get(), ptrs[dis(gen)].Get()));
    }
  }
  ASSERT_EQ(S::cnt, kTotal);
  landfill::GC();
  ASSERT_EQ(S::cnt, kTotal);
  ptrs.clear();
  landfill::GC();
  ASSERT_EQ(S::cnt, 0);
}
