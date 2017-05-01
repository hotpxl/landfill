#include <gtest/gtest.h>
#include <landfill/types.h>

struct S : public landfill::Collectible<S> {
 public:
  static int cnt;
  S() : landfill::Collectible<S>{this} { ++cnt; }
  ~S() { --cnt; }
};  // struct S

int S::cnt = 0;

TEST(StrongPointerTest, IncrementCounter) {
  S* ptr = new S{};
  landfill::StrongPointer<S> s{ptr};
  ASSERT_EQ(S::cnt, 1);
  delete ptr;
}

TEST(StrongPointerTest, DecrementCounter) {
  {
    landfill::StrongPointer<S> s{new S{}};
    ASSERT_EQ(S::cnt, 1);
  }
  landfill::GC();
  ASSERT_EQ(S::cnt, 0);
}
