#include <landfill/types.h>

struct S : public landfill::Collectible<S> {
 public:
  S() : landfill::Collectible<S>{this} {}
};

int main() {
  S* s = new S{};
  landfill::StrongPointer<S> a{s};
  return 0;
}
