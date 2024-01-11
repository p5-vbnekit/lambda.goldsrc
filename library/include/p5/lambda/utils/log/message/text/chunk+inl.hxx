#pragma once

#include <tuple>
#include <string>
#include <utility>
#include <string_view>
#include <type_traits>

#include "chunk.hxx"


namespace p5::lambda::utils::log::message::text::chunk {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

inline static parent_::Type make() noexcept(true) { return {}; }

inline static parent_::Type make(char const *source) noexcept(true) {
    if (source) return {.reference = source, .terminated = true};
    return {};
}

inline static parent_::Type make(::std::string const &source) noexcept(true) {
    return {.reference = source, .terminated = true};
}

inline static parent_::Type make(
    ::std::string_view &&source, bool terminated = false
) noexcept(true) {
    return {.reference = ::std::move(source), .terminated = terminated};
}

inline static parent_::Type make(
    ::std::string_view const &source, bool terminated = false
) noexcept(true) {
    return {.reference = source, .terminated = terminated};
}

} // namespace private_

template <class ... T> inline constexpr auto
make(T && ... source) noexcept(true) requires([] {
    if constexpr (1 < sizeof ... (T)) {
        if constexpr (! ::std::is_base_of_v<::std::string_view, ::std::decay_t<
            ::std::tuple_element_t<0, ::std::tuple<T ...>>
        >>) return false;
    }
    return requires () { private_::make(::std::declval<T>() ...); };
} ()) {
    return this_::private_::make(::std::forward<T>(source) ...);
}

} // namespace p5::lambda::utils::log::message::text
