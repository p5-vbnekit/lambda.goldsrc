#pragma once

#include <type_traits>

#include "type+fwd.hxx"
#include "source+fwd.hxx"
#include "handler+fwd.hxx"
#include "dispatcher+fwd.hxx"
#include "subscription+fwd.hxx"


namespace p5::lambda::utils::event::dispatcher {

using Subscription = parent_::Subscription;

template <class ... T> using Base = parent_::Source<T ...>;
template <class ... T> using Handler = parent_::Handler<T ...>;

template <class T> struct Type final: Base<T> {
    using Base = this_::Base<T>;
    using Event = parent_::Type<T>;
    using Handler = this_::Handler<Event>;
    using Subscription = this_::Subscription;

    template <class ... Event> auto dispatch(Event && ...);

    Type() noexcept(false);
    Type(Type &&) = default;
    Type & operator = (Type &&) = default;

private:
    this_::Driver_ &driver_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;

    explicit Type(::std::decay_t<decltype(driver_)> *driver) noexcept(false);
};

} // namespace p5::lambda::utils::event::dispatcher


#include "dispatcher+inl.hxx"
