#include <landfill/types.h>
#include <algorithm>
#include <vector>

namespace landfill {

namespace {

std::unordered_set<RawStrongPointer*> root_set{};

void AddToRootSet(RawStrongPointer* ptr) {
  root_set.emplace(ptr);
}

void RemoveFromRootSet(RawStrongPointer* ptr) {
  root_set.erase(ptr);
}

}  // anonymous namespace

void GC() {
}

RawStrongPointer::RawStrongPointer() {
  AddToRootSet(this);
}

RawStrongPointer::RawStrongPointer(RawCollectible* ref) : ref_{ref} {
  AddToRootSet(this);
}

RawStrongPointer::RawStrongPointer(RawStrongPointer const& other) : ref_{other.ref_} {
  AddToRootSet(this);
}

RawStrongPointer::RawStrongPointer(RawStrongPointer&& other) : ref_{other.ref_} {
  AddToRootSet(this);
}

RawStrongPointer::~RawStrongPointer() {
  RemoveFromRootSet(this);
}

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

RawCollectible* RawStrongPointer::GetUntyped() const {
  return ref_;
}

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

RawCollectible* RawWeakPointer::GetUntyped() const { return ref_; }

void RawWeakPointer::Reset() { ref_ = nullptr; }

void RawWeakPointer::Reset(RawCollectible* ptr) { ref_ = ptr; }

RawCollectible::RawCollectible(void* self) : self_{self} {}

void* RawCollectible::GetUntypedSelf() { return self_; }

void RawCollectible::AddPointer(RawWeakPointer* ptr) { pointers_.emplace(ptr); }

void RawCollectible::RemovePointer(RawWeakPointer* ptr) {
  pointers_.erase(ptr);
}

}  // namespace landfill
