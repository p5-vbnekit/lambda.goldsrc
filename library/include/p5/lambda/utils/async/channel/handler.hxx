#pragma once

#include <functional>
#include <type_traits>

#include "event.hxx"
#include "handler+fwd.hxx"


namespace p5::lambda::utils::async::channel::handler::type {

template <class Event> struct Wrapper final {
    static_assert(! ::std::is_void_v<Event>);
    using Type = ::std::function<void(parent_::parent_::Event<Event> const &)>;
};

template <> struct Wrapper<void> final {
    using Type = ::std::function<void(parent_::parent_::Event<void>)>;
};

} // namespace p5::lambda::utils::async::channel::handler::type
