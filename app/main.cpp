#include <landfill/types.h>
#include <cstdio>
#include <vector>

struct P : public landfill::Collectible<P> {
  P() { std::printf("P::P()\n"); }
  ~P() { std::printf("P::~P()\n"); }
};

struct S : public landfill::Collectible<S> {
  S() { std::printf("S::S()\n"); }
  ~S() { std::printf("S::~S()\n"); }
  std::vector<landfill::WeakPointer<P>> ptrs;
};

int main() {
  landfill::GC();
  {
    landfill::StrongPointer<S> a{new S{}};
    landfill::StrongPointer<P> b{new P{}};
    landfill::GC();
    a->ptrs.push_back(landfill::WeakPointer<P>{a.Get(), b.Get()});
    landfill::GC();
  }
  landfill::GC();
  return 0;
}
