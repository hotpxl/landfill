#include <landfill/types.h>
#include <vector>
#include <algorithm>

namespace landfill {

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

RawCollectible* RawWeakPointer::Get() const { return ref_; }

void RawWeakPointer::Reset() { ref_ = nullptr; }

void RawWeakPointer::Reset(RawCollectible* ptr) { ref_ = ptr; }

RawCollectible::RawCollectible(RawCollectible& other) {
  for (auto&& ptr : other.pointers_) {
    pointers_.emplace(this, ptr->Get());
  }
}

RawCollectible::RawCollectible(RawCollectible&& other) {
  for (auto&& ptr : other.pointers_) {
    pointers_.emplace(this, ptr->Get());
  }
}

RawCollectible& RawCollectible::operator=(RawCollectible const& other) {
}

}  // namespace landfill
