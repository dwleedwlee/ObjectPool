# ObjectPool

The object pool is a set of pre allocated objects.
The object pool returns object address from the statically allocated address space.

Because the embedded system with RTOS prohibits from allocating memory address dynamically from heap,
it is required to allocate and free objects with safe manner in predictable time.

The idea of constructing an object pool with generic type comes from the list.h source code of the Linux
