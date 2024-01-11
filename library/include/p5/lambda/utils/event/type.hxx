#pragma once

#include <type_traits>

#include "type+fwd.hxx"


namespace p5::lambda::utils::event::type {

template <class T> struct Wrapper final {
    using Type = ::std::decay_t<T>;
    static_assert(::std::is_same_v<T, Type>);
};

} // namespace p5::lambda::utils::event::type
