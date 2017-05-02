# Landfill

Just as the name suggests, this is a [garbage
collector](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science))
for C++. It automatically manages objects allocated by the user
through the interface, freeing them when not reachable. It aims to be
sound and complete. Of course its completeness is defined on
*reachability* instead of the more strict *liveness*.

## Algorithm

Currently it uses a stop-the-world version of mark-sweep. But I intend
to make it a concurrent, tri-color, mark-sweep garbage collector
eventually.

All pointers are wrapped either in `StrongPointer` or
`WeakPointer`. Think `StrongPointer` as the root set. So you would
keep everything you directly reference as `StrongPointer`. And keep
every indirect reference as `WeakPointer`. When the garbage collector
is cleaning up, it will only start traversing from `StrongPointer`s
and then take reachability closure for subsequent `StrongPointer`s and
`WeakPointer`s.