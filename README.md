# Landfill

Just as the name suggests, this is a [Garbage collector](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)) for C++. It automatically manages objects allocated by the user through the interface, freeing them when not reachable. It aims to be sound and complete. Of course its completeness is defined on *reachability* instead of the more strict *liveness*.
