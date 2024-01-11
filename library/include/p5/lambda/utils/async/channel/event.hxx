#pragma once

#include <type_traits>

#include "event+fwd.hxx"


namespace p5::lambda::utils::async::channel::event::type {

template <class T> struct Wrapper final {
    using Type = ::std::remove_const_t<::std::remove_reference_t<T>>;
    static_assert(::std::is_same_v<T, Type>);
};

} // namespace p5::lambda::utils::async::channel::event::type
