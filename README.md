## XVECS
The following library is an Archetypal Entity Component System (ECS) written in C++20 as a Single Header Library.

This library draws inspiration from Unity DOTS with a different underlying storage structure.

**The ECS is designed to achieve the following:**
- Support for running multiple world instances and running them in parallel with syncing. Hence, it allows categorisation into specialised worlds like GUI World, Physics World, Game World, and Rendering World.
- Parallelism at the system level, so systems within each world can update in parallel.
- High performance through data-oriented design geared at data and code caching.
- Compile-time static polymorphism, no virtual table and jumps.
- Remove the cost of tagging component in Archetypal ECS that usually results in moving data between different archetype's tables to maintain cache coherency.
- Built-in support for a hierarchical relationship between entities.
- Built-in support for Prefab/Blueprint/Prototype entities.
- Offer two types of storage, table storage and sparse-set storage, toggled at compile time.

Unlike Unity's ECS, XVECS will not support Shared Components as the design tends to thrash the memory when there are too many shared components.
Instead of opting for shared components, developers can usually maintain a hierarchical relationship between entities to reuse data, which avoids thrashing the memory.

## Status
The current library is a heavy WIP as I am rewriting the design in support of making the tagging free and removing the implementation for shared components.