#ifndef LANDFILL_LANDFILL_TYPES_H_
#define LANDFILL_LANDFILL_TYPES_H_
#include <unordered_set>

namespace landfill {

template <typename T>
class StrongPointer {};  // class StrongPointer

template <typename T>
class WeakPointer {};  // class WeakPointer

template <typename T>
class Collectible {
 protected:
  AddPointer(WeakPointer<T>* ptr);
  RemovePointer(WeakPointer<T>* ptr);

 private:
  std::unordered_set<WeakPointer<T>*> pointers_;
};  // class Collectible;

}  // namespace landfill

#endif  // LANDFILL_LANDFILL_TYPES_H_
