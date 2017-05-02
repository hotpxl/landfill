# Landfill

Just as the name suggests, this is a [garbage
collector](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))
for C++. It automatically manages objects allocated by the user
through the interface, freeing them when not reachable. It aims to be
sound and complete. Of course its completeness is defined on
*reachability* instead of the more strict *liveness*.

## Algorithm and Implementation

Currently it uses a stop-the-world version of mark-sweep. But I intend
to make it a concurrent, tri-color, mark-sweep garbage collector
eventually.

All pointers are wrapped either in `StrongPointer` or
`WeakPointer`. Think `StrongPointer`s as the root set. So you would
keep everything you directly reference as `StrongPointer`. And keep
every indirect reference as `WeakPointer`. When the garbage collector
is cleaning up, it will only start traversing from `StrongPointer`s
and then take reachability closure for subsequent `StrongPointer`s and
`WeakPointer`s.

In fact, `StrongPointer` inherits from `RawStrongPointer` and
`WeakPointer` inherits from `RawWeakPointer`. They both serve as typed
wrappers to hide *unsoundness* (in the sense of type system) under the
rug.

To make objects collectible, they have to inherit from
`Collectible<T>` using a *curiously recurring template pattern*. And
they need to pass down `this` during construction. The next code
snippet serves as an example.

```cpp
struct MyObject : public landfill::Collection<MyObject> {
  // ...
};
```

In fact, `Collectible` inherits from `RawCollectible` virtually, so
multiple inheritance with each base class collectible is also
supported. In addition, `RawCollectible` declares a virtual
destructor, so it will always call the correct destructor. It means if
some class inherits from another collectible class, but is not itself
collectible, then this class will be properly garbage collected, only
that it cannot be pointed to using `StrongPointer` or `WeakPointer`.
Another implementation would not make destructor virtual, thus
possibly saving the vtable, but will have to store a function pointer
with each `RawCollectible` instance for destruction. By doing this, we
are trading the vtable pointer for a function pointer, which is more
or less equivalent.

## Usage

As aliases, use `S` for `StrongPointer`, `W` for `WeakPointer`, `C`
for `Collectible`.

Inherit from `Collectible` if you want to point to it.

```cpp
struct O : landfill::C<O> {
  // ...
};
```

`StrongPointer` can be made directly.

```cpp
auto strong_pointer = landfill::S<O>::Make();
```

`WeakPointer` can be stored as an indirect reference, but must be
initialized with the owner.

```cpp
struct O2 : landfill::C<O2> {
  landfill::W<O> to_o;
  O2() : to_o{this} {}
};
```
