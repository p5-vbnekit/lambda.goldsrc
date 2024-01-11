#pragma once

#include <type_traits>

#include "game.hxx"


namespace p5::lambda::game {

inline constexpr auto api::version(
    parent_::functions::Standard const *
) noexcept(true) { return 140; }

inline constexpr auto api::version(
    parent_::functions::Extension const *
) noexcept(true) { return 1; }

template <class T> inline consteval auto api::version() noexcept(true) {
    return version(static_cast<::std::decay_t<T> const *>(nullptr));
}

} // namespace p5::lambda::game
