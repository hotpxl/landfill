#include <gtest/gtest.h>
#include <landfill/types.h>

namespace {
struct B0 : public landfill::Collectible<B0> {
 public:
  static int cnt;
  B0() : landfill::Collectible<B0>{this} { ++cnt; }
  virtual ~B0() { --cnt; }
  int data = 0;
};  // struct B0

struct Dummy {
  virtual ~Dummy() = default;
  int data = 0;
};  // struct Dummy

struct Derived : public Dummy, public B0 {
  static int cnt;
  Derived() : Dummy{}, B0{} { ++cnt; }
  virtual ~Derived() { --cnt; }
  int data = 0;
};  // struct Derived;

int B0::cnt = 0;
int Derived::cnt = 0;
}

TEST(Inheritance, MultipleInheritanceDataLayout) {
  Derived* d = new Derived{};
  B0* b = d;
  ASSERT_NE(static_cast<void*>(b), static_cast<void*>(d));
  delete d;
}

TEST(Inheritance, MultipleInheritance) {
  {
    landfill::StrongPointer<B0> d{new Derived{}};
    ASSERT_EQ(B0::cnt, 1);
    ASSERT_EQ(Derived::cnt, 1);
    landfill::GC();
    ASSERT_EQ(B0::cnt, 1);
    ASSERT_EQ(Derived::cnt, 1);
  }
  landfill::GC();
  ASSERT_EQ(B0::cnt, 0);
  ASSERT_EQ(Derived::cnt, 0);
}
