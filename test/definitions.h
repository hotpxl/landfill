#ifndef LANDFILL_TEST_DEFINITIONS_H_
#define LANDFILL_TEST_DEFINITIONS_H_
#include <landfill/types.h>
#include <vector>

struct S : public landfill::Collectible<S> {
 public:
  static int cnt;
  S() { ++cnt; }
  ~S() { --cnt; }
  std::vector<landfill::WeakPointer<S>> ptrs;
};  // struct S

#endif  // LANDFILL_TEST_DEFINITIONS_H_
