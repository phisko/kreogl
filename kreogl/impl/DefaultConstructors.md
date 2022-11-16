# [DefaultConstructors](DefaultConstructors.hpp)

Macros to reduce boilerplate code when needing to explicitly default or delete constructors.

## Members

### `KREOGL_MOVE_ONLY`

```cpp
#define KREOGL_MOVE_ONLY(T) \
	T() noexcept = default; \
	KREOGL_DELETE_COPY(T) \
	KREOGL_DEFAULT_MOVE(T)
```

Marks `T` as move-only, with a default ctor, default move constructor and assignment operator, and deleted copy constructor and assignment operator.

### `KREOGL_DEFAULT_COPY`

```cpp
#define KREOGL_DEFAULT_COPY(T) \
	T(const T &) noexcept = default; \
	T & operator=(const T &) noexcept = default;
```

Defaults the copy constructor and assignment operator for `T`.

### `KREOGL_DELETE_COPY`

```cpp
#define KREOGL_DELETE_COPY(T) \
	T(const T &) noexcept = delete; \
	T & operator=(const T &) noexcept = delete;
```

Deletes the copy constructor and assignment operator for `T`.

### `KREOGL_DEFAULT_MOVE`

```cpp
#define KREOGL_DEFAULT_MOVE(T) \
	T(T &&) noexcept = default; \
	T & operator=(T &&) noexcept = default;
```

Defaults the move constructor and assignment operator for `T`.

### `KREOGL_DELETE_MOVE`

```cpp
#define KREOGL_DELETE_MOVE(T) \
	T(T &&) noexcept = delete; \
	T & operator=(T &&) noexcept = delete;
```

Deletes the move constructor and assignment operator for `T`.
