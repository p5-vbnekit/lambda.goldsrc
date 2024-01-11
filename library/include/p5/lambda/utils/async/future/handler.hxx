#pragma once

#include <functional>
#include <type_traits>

#include "value.hxx"
#include "handler+fwd.hxx"


namespace p5::lambda::utils::async::future::handler::type {

template <class T> struct Wrapper final {
    static_assert(::std::is_same_v<parent_::parent_::Value<T>, T>);
    using Type = ::std::function<void(void)>;
};

} // namespace p5::lambda::utils::async::future::handler::type
