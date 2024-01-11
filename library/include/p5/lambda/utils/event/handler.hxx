#pragma once

#include <functional>
#include <type_traits>

#include "handler+fwd.hxx"

#include "type.hxx"


namespace p5::lambda::utils::event::handler::type {

template <class T> struct Wrapper final {
    using Type = ::std::function<void(parent_::parent_::Type<T> const &)>;
};

template <> struct Wrapper<void> final {
    using Type = ::std::function<void(void)>;
};

} // namespace p5::lambda::utils::event::handler::type
