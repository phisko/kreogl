#pragma once

// clang-format off
#define KREOGL_DEFAULT_COPY(T) \
	T(const T &) noexcept = default; \
	T & operator=(const T &) noexcept = default;

#define KREOGL_DELETE_COPY(T) \
	T(const T &) noexcept = delete; \
	T & operator=(const T &) noexcept = delete;

#define KREOGL_DEFAULT_MOVE(T) \
	T(T &&) noexcept = default; \
	T & operator=(T &&) noexcept = default;

#define KREOGL_DELETE_MOVE(T) \
	T(T &&) noexcept = delete; \
	T & operator=(T &&) noexcept = delete;

#define KREOGL_MOVE_ONLY(T) \
	T() noexcept = default; \
	KREOGL_DELETE_COPY(T) \
	KREOGL_DEFAULT_MOVE(T)
// clang-format on