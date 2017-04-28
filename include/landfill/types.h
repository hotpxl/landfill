#ifndef LANDFILL_LANDFILL_TYPES_H_
#define LANDFILL_LANDFILL_TYPES_H_
#include <unordered_set>

namespace landfill {

template <typename T>
class Collectible;

class RawCollectible;

template <typename T>
class StrongPointer {
 public:
  StrongPointer() = default;
  StrongPointer(Collectible<T>* c);
  StrongPointer(StrongPointer<T> const& other) = default;
  StrongPointer(StrongPointer<T>&& other) = default;
  virtual ~StrongPointer() = default;
  StrongPointer& operator=(StrongPointer const& other) = default;
  StrongPointer& operator=(StrongPointer&& other) = default;
  T* Get() const;
  T& operator*() const;
  T* operator->() const;

 private:
  Collectible<T>* ref_{nullptr};
};  // class StrongPointer

class RawWeakPointer {
 public:
  RawWeakPointer() = delete;
  RawWeakPointer(RawCollectible* owner);
  RawWeakPointer(RawCollectible* owner, RawCollectible* ref);
  RawWeakPointer(RawWeakPointer const& other);
  RawWeakPointer(RawWeakPointer&& other);
  virtual ~RawWeakPointer();
  RawWeakPointer& operator=(RawWeakPointer const& other);
  RawWeakPointer& operator=(RawWeakPointer&& other);
  RawCollectible* Get() const;
  void Reset();
  void Reset(RawCollectible* ptr);

 private:
  RawCollectible* owner_;
  RawCollectible* ref_;
};  // class RawWeakPointer

class RawCollectible {
 public:
  RawCollectible() = default;
  RawCollectible(RawCollectible& other);
  RawCollectible(RawCollectible&& other);
  virtual ~RawCollectible() = default;
  RawCollectible& operator=(RawCollectible const& other);
  RawCollectible& operator=(RawCollectible&& other);
  void AddPointer(RawWeakPointer* ptr);
  void RemovePointer(RawWeakPointer* ptr);

 private:
  std::unordered_set<RawWeakPointer*> pointers_;
};  // class RawCollectible

template <typename T, typename U>
class WeakPointer : RawWeakPointer {
 public:
  WeakPointer() = delete;
  WeakPointer(Collectible<U>* owner);
  WeakPointer(Collectible<U>* owner, Collectible<T>* ref);
  WeakPointer(WeakPointer<T, U> const& other);
  WeakPointer(WeakPointer<T, U>&& other);
  virtual ~WeakPointer();
  WeakPointer<T, U>& operator=(WeakPointer<T, U> const& other);
  WeakPointer<T, U>& operator=(WeakPointer<T, U>&& other);
  T* Get() const;
  T& operator*() const;
  T* operator->() const;
  void Reset();
  void Reset(Collectible<T>* ptr);

 private:
  Collectible<T>* ref_{nullptr};
  Collectible<U>* owner_{nullptr};
};  // class WeakPointer

template <typename T>
class Collectible {
 protected:
  template <typename R>
  void AddPointer(WeakPointer<R, T>* ptr);
  template <typename R>
  void RemovePointer(WeakPointer<R, T>* ptr);
  T* GetSelf();

 private:
  std::unordered_set<RawWeakPointer> pointers_;
  T* self_;
};  // class Collectible;

template <typename T>
StrongPointer<T>::StrongPointer(Collectible<T>* c) : ref_{c} {}

template <typename T>
T* StrongPointer<T>::Get() const {
  return ref_->GetSelf();
}

template <typename T>
T& StrongPointer<T>::operator*() const {
  return *Get();
}

template <typename T>
T* StrongPointer<T>::operator->() const {
  return Get();
}

template <typename T, typename U>
WeakPointer<T, U>::WeakPointer(Collectible<U>* owner) : owner_{owner} {
  owner_->AddPointer(this);
}

template <typename T, typename U>
WeakPointer<T, U>::WeakPointer(Collectible<U>* owner, Collectible<T>* ref)
    : WeakPointer{owner}, ref_{ref} {}

template <typename T, typename U>
WeakPointer<T, U>::WeakPointer(WeakPointer<T, U> const& other)
    : WeakPointer{other.owner_}, ref_{other.ref_} {}

template <typename T, typename U>
WeakPointer<T, U>::WeakPointer(WeakPointer<T, U>&& other)
    : WeakPointer{other.owner_}, ref_{other.ref_} {}

template <typename T, typename U>
WeakPointer<T, U>::~WeakPointer() {
  owner_->RemovePointer(this);
}

template <typename T, typename U>
WeakPointer<T, U>& WeakPointer<T, U>::operator=(WeakPointer<T, U> const& other) {
  if (&other == this) {
    return *this;
  }
  ref_ = other.ref_;
  return *this;
}

template <typename T, typename U>
WeakPointer<T, U>& WeakPointer<T, U>::operator=(WeakPointer<T, U>&& other) {
  if (&other == this) {
    return *this;
  }
  ref_ = other.ref_;
  return *this;
}

template <typename T, typename U>
T* WeakPointer<T, U>::Get() const {
  return ref_->GetSelf();
}

template <typename T, typename U>
T& WeakPointer<T, U>::operator*() const {
  return *Get();
}

template <typename T, typename U>
T* WeakPointer<T, U>::operator->() const {
  return Get();
}

template <typename T, typename U>
void WeakPointer<T, U>::Reset() {
  ref_ = nullptr;
}

template <typename T, typename U>
void WeakPointer<T, U>::Reset(Collectible<T>* ptr) {
  ref_ = ptr;
}

template <typename T>
void Collectible<T>::AddPointer(WeakPointer<T>* ptr) {
  pointers_.emplace(ptr);
}

template <typename T>
void Collectible<T>::RemovePointer(WeakPointer<T>* ptr) {
  pointers_.erase(ptr);
}

}  // namespace landfill

#endif  // LANDFILL_LANDFILL_TYPES_H_
