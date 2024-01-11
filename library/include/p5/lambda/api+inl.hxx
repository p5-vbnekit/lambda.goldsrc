#pragma once

#include <type_traits>

#include "api.hxx"


namespace p5::lambda::api {

inline constexpr auto version(
    parent_::engine::Functions const *
) noexcept(true) {
    return parent_::engine::api::version();
}

inline constexpr auto version(
    parent_::game::functions::Standard const *pointer
) noexcept(true) {
    return parent_::game::api::version(pointer);
}

inline constexpr auto version(
    parent_::game::functions::Extension const *pointer
) noexcept(true) {
    return parent_::game::api::version(pointer);
}

inline constexpr auto version(
    parent_::metamod::Functions const *pointer
) noexcept(true) {
    return parent_::metamod::api::version(pointer);
}

inline constexpr auto version(
    parent_::metamod::plugin::Functions const *pointer
) noexcept(true) {
    return parent_::metamod::api::version(pointer);
}

template <class T> inline consteval auto version() noexcept(true) {
    return version(static_cast<::std::decay_t<T> const *>(nullptr));
}

} // namespace p5::lambda::api
