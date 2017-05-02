#include <gtest/gtest.h>
#include <landfill/types.h>

namespace {
struct B0 : public landfill::Collectible<B0> {
 public:
  static int cnt;
  B0() { ++cnt; }
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
}  // anonymous namespace

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

namespace {
struct B : landfill::C<B> {
  int b = 0;
};  // struct B

struct C : landfill::C<C> {
  int c = 0;
};  // struct C
struct A : landfill::C<A>, B, C {
  int a = 0;
};  // struct A

}  // anonymous namespace

TEST(Inheritance, MultipleInheritanceAccess) {
  auto a = new A{};
  auto a_ptr = landfill::S<A>(a);
  auto b_ptr = landfill::S<B>(a);
  auto c_ptr = landfill::S<C>(a);
  ASSERT_EQ(a_ptr->a, 0);
  ASSERT_EQ(a_ptr->b, 0);
  ASSERT_EQ(a_ptr->c, 0);
  ASSERT_EQ(b_ptr->b, 0);
  ASSERT_EQ(c_ptr->c, 0);
  c_ptr->c = 2;
  b_ptr->b = 1;
  a_ptr->a = -1;
  ASSERT_EQ(a_ptr->a, -1);
  ASSERT_EQ(a_ptr->b, 1);
  ASSERT_EQ(a_ptr->c, 2);
  ASSERT_EQ(b_ptr->b, 1);
  ASSERT_EQ(c_ptr->c, 2);
}
