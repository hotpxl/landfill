#ifndef LANDFILL_LANDFILL_TYPES_H_
#define LANDFILL_LANDFILL_TYPES_H_
#include <type_traits>
#include <unordered_set>

namespace landfill {

void GC();

class RawCollectible;

class RawWeakPointer {
 public:
  RawWeakPointer() = delete;
  RawWeakPointer(RawCollectible* owner);
  RawWeakPointer(RawCollectible* owner, RawCollectible* ref);
  RawWeakPointer(RawWeakPointer const& other);
  RawWeakPointer(RawWeakPointer&& other);
  ~RawWeakPointer();
  RawWeakPointer& operator=(RawWeakPointer const& other);
  RawWeakPointer& operator=(RawWeakPointer&& other);
  RawCollectible* GetCollectible() const;
  void Reset();
  void Reset(RawCollectible* ptr);

 private:
  RawCollectible* owner_{nullptr};
  RawCollectible* ref_{nullptr};
};  // class RawWeakPointer

class RawStrongPointer {
 public:
  RawStrongPointer();
  RawStrongPointer(RawCollectible* ref);
  RawStrongPointer(RawStrongPointer const& other);
  RawStrongPointer(RawStrongPointer&& other);
  ~RawStrongPointer();
  RawStrongPointer& operator=(RawStrongPointer const& other);
  RawStrongPointer& operator=(RawStrongPointer&& other);
  RawCollectible* GetCollectible() const;
  void Reset();
  void Reset(RawCollectible* ptr);

 private:
  RawCollectible* ref_{nullptr};
};  // class RawStrongPointer

class RawCollectible {
 public:
  RawCollectible() = delete;
  RawCollectible(void* self);
  RawCollectible(RawCollectible const& other) = delete;
  RawCollectible(RawCollectible&& other) = delete;
  virtual ~RawCollectible();
  RawCollectible& operator=(RawCollectible const& other) = delete;
  RawCollectible& operator=(RawCollectible&& other) = delete;
  void* GetUntypedSelf();
  // Do not call these functions directly.
  void AddPointer(RawWeakPointer* ptr);
  void RemovePointer(RawWeakPointer* ptr);
  std::unordered_set<RawWeakPointer*> GetReferences();

 private:
  std::unordered_set<RawWeakPointer*> pointers_;
  void* self_;
};  // class RawCollectible

template <typename T>
class Collectible;

template <typename T>
class StrongPointer : public RawStrongPointer {
  static_assert(std::is_base_of<Collectible<T>, T>::value,
                "Must inherit from Collectible.");

 public:
  StrongPointer() = default;
  StrongPointer(Collectible<T>* c);
  StrongPointer(StrongPointer<T> const& other) = default;
  StrongPointer(StrongPointer<T>&& other) = default;
  ~StrongPointer() = default;
  StrongPointer& operator=(StrongPointer const& other) = default;
  StrongPointer& operator=(StrongPointer&& other) = default;
  T* Get() const;
  T& operator*() const;
  T* operator->() const;
};  // class StrongPointer

template <typename T, typename U>
class WeakPointer : public RawWeakPointer {
  static_assert(std::is_base_of<Collectible<T>, T>::value,
                "Must inherit from Collectible.");
  static_assert(std::is_base_of<Collectible<U>, U>::value,
                "Must inherit from Collectible.");

 public:
  WeakPointer() = delete;
  WeakPointer(Collectible<U>* owner);
  WeakPointer(Collectible<U>* owner, Collectible<T>* ref);
  WeakPointer(WeakPointer<T, U> const& other) = default;
  WeakPointer(WeakPointer<T, U>&& other) = default;
  ~WeakPointer() = default;
  WeakPointer<T, U>& operator=(WeakPointer<T, U> const& other) = default;
  WeakPointer<T, U>& operator=(WeakPointer<T, U>&& other) = default;
  T* Get() const;
  T& operator*() const;
  T* operator->() const;
  void Reset(Collectible<T>* ptr);
};  // class WeakPointer

template <typename T>
class Collectible : public RawCollectible {
 public:
  Collectible() = delete;
  Collectible(T* self);
  Collectible(Collectible<T> const& other) = delete;
  Collectible(Collectible<T>&& other) = delete;
  ~Collectible() = default;
  Collectible<T>& operator=(Collectible<T> const& other) = delete;
  Collectible<T>& operator=(Collectible<T>&& other) = delete;
  T* GetSelf();
};  // class Collectible;

template <typename T>
StrongPointer<T>::StrongPointer(Collectible<T>* c) : RawStrongPointer{c} {}

template <typename T>
T* StrongPointer<T>::Get() const {
  return static_cast<T*>(GetCollectible()->GetUntypedSelf());
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
WeakPointer<T, U>::WeakPointer(Collectible<U>* owner)
    : RawWeakPointer{owner} {};

template <typename T, typename U>
WeakPointer<T, U>::WeakPointer(Collectible<U>* owner, Collectible<T>* ref)
    : RawWeakPointer{owner, ref} {};

template <typename T, typename U>
T* WeakPointer<T, U>::Get() const {
  return static_cast<T*>(GetCollectible()->GetUntypedSelf());
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
void WeakPointer<T, U>::Reset(Collectible<T>* ptr) {
  Reset(static_cast<RawCollectible*>(ptr));
}

template <typename T>
Collectible<T>::Collectible(T* self) : RawCollectible{self} {};

template <typename T>
T* Collectible<T>::GetSelf() {
  return static_cast<T*>(GetUntypedSelf());
}

}  // namespace landfill

#endif  // LANDFILL_LANDFILL_TYPES_H_
