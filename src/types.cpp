#include <landfill/types.h>
#include <algorithm>
#include <vector>

namespace landfill {

namespace {

std::unordered_set<RawStrongPointer*> root_set{};
std::unordered_set<RawCollectible*> live_objects{};

void AddToRootSet(RawStrongPointer* ptr) { root_set.emplace(ptr); }

void RemoveFromRootSet(RawStrongPointer* ptr) { root_set.erase(ptr); }

void AddToLiveObjects(RawCollectible* ptr) { live_objects.emplace(ptr); }

void RemoveFromLiveObjects(RawCollectible* ptr) { live_objects.erase(ptr); }

}  // anonymous namespace

void GC() {
  // std::printf("Rootset: %zu\n", root_set.size());
  // for (auto p : root_set) {
  //   std::printf("%p\n", p);
  // }
  // std::printf("Live objects: %zu\n", live_objects.size());
  // for (auto p : live_objects) {
  //   std::printf("%p\n", p);
  // }
  std::vector<RawCollectible*> stack;
  std::unordered_set<RawCollectible*> reachable;
  for (auto p : root_set) {
    auto ref = p->GetCollectible();
    if (ref == nullptr) {
      continue;
    }
    stack.push_back(ref);
    reachable.emplace(ref);
  }
  while (!stack.empty()) {
    auto p = stack.back();
    stack.pop_back();
    for (auto ptr : p->GetReferences()) {
      auto ref = ptr->GetCollectible();
      if (ref == nullptr) {
        continue;
      }
      if (reachable.count(ref) == 0) {
        reachable.emplace(ref);
        stack.push_back(ref);
      }
    }
  }
  auto live_objects_copy = live_objects;
  for (auto obj : live_objects_copy) {
    if (reachable.count(obj) == 0) {
      // std::printf("%p is not reachable.\n", obj);
      delete obj;
    }
  }
}

RawStrongPointer::RawStrongPointer() { AddToRootSet(this); }

RawStrongPointer::RawStrongPointer(RawCollectible* ref) : ref_{ref} {
  AddToRootSet(this);
}

RawStrongPointer::RawStrongPointer(RawStrongPointer const& other)
    : ref_{other.ref_} {
  AddToRootSet(this);
}

RawStrongPointer::RawStrongPointer(RawStrongPointer&& other)
    : ref_{other.ref_} {
  AddToRootSet(this);
}

RawStrongPointer::~RawStrongPointer() { RemoveFromRootSet(this); }

RawStrongPointer& RawStrongPointer::operator=(RawStrongPointer const& other) {
  if (&other == this) {
    return *this;
  }
  ref_ = other.ref_;
  return *this;
}

RawStrongPointer& RawStrongPointer::operator=(RawStrongPointer&& other) {
  if (&other == this) {
    return *this;
  }
  ref_ = other.ref_;
  return *this;
}

RawCollectible* RawStrongPointer::GetCollectible() const { return ref_; }

void RawStrongPointer::Reset() { ref_ = nullptr; }

void RawStrongPointer::Reset(RawCollectible* ptr) { ref_ = ptr; }

RawWeakPointer::RawWeakPointer(RawCollectible* owner) : owner_{owner} {
  owner_->AddPointer(this);
}

RawWeakPointer::RawWeakPointer(RawCollectible* owner, RawCollectible* ref)
    : owner_{owner}, ref_{ref} {
  owner_->AddPointer(this);
}

RawWeakPointer::RawWeakPointer(RawWeakPointer const& other)
    : owner_{other.owner_}, ref_{other.ref_} {
  owner_->AddPointer(this);
}

RawWeakPointer::RawWeakPointer(RawWeakPointer&& other)
    : owner_{other.owner_}, ref_{other.ref_} {
  owner_->AddPointer(this);
}

RawWeakPointer::~RawWeakPointer() { owner_->RemovePointer(this); }

RawWeakPointer& RawWeakPointer::operator=(RawWeakPointer const& other) {
  if (&other == this) {
    return *this;
  }
  ref_ = other.ref_;
  return *this;
}

RawWeakPointer& RawWeakPointer::operator=(RawWeakPointer&& other) {
  if (&other == this) {
    return *this;
  }
  ref_ = other.ref_;
  return *this;
}

RawCollectible* RawWeakPointer::GetCollectible() const { return ref_; }

void RawWeakPointer::Reset() { ref_ = nullptr; }

void RawWeakPointer::Reset(RawCollectible* ptr) { ref_ = ptr; }

RawCollectible::RawCollectible() {
  AddToLiveObjects(this);
}

RawCollectible::~RawCollectible() { RemoveFromLiveObjects(this); }

void RawCollectible::AddPointer(RawWeakPointer* ptr) { pointers_.emplace(ptr); }

void RawCollectible::RemovePointer(RawWeakPointer* ptr) {
  pointers_.erase(ptr);
}

std::unordered_set<RawWeakPointer*> RawCollectible::GetReferences() {
  return pointers_;
}

}  // namespace landfill
