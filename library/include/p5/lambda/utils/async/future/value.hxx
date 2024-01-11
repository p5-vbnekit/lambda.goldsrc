#pragma once

#include <type_traits>

#include "value+fwd.hxx"


namespace p5::lambda::utils::async::future::value::type {

template <class T> struct Wrapper final {
    static_assert(! std::is_rvalue_reference_v<T>);
    using Type = ::std::remove_reference_t<T>;
};

} // namespace p5::lambda::utils::async::future::value::type
