#pragma once

#include <type_traits>

#include "metamod.hxx"


namespace p5::lambda::metamod {

inline constexpr auto const * api::version(
    parent_::Functions const *
) noexcept(true) { return api::version(); }

inline constexpr auto const * api::version(
    parent_::plugin::Functions const *
) noexcept(true) { return api::version(); }

template <class T> inline consteval auto const * api::version() noexcept(true) {
    return api::version(static_cast<::std::decay_t<T> const *>(nullptr));
}

} // namespace p5::lambda::metamod
