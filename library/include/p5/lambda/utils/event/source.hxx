#pragma once

#include <memory>
#include <type_traits>

#include "type+fwd.hxx"
#include "source+fwd.hxx"
#include "handler+fwd.hxx"
#include "subscription+fwd.hxx"


namespace p5::lambda::utils::event::source {

namespace base {

struct Type {
    virtual ~Type() noexcept(true);

protected:
    Type() = default;
    Type(Type &&) = default;
    Type & operator = (Type &&) = default;

private:
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace base

using Subscription = parent_::Subscription;

template <class ... T> using Handler = parent_::Handler<T ...>;

template <class T> struct Type: Base {
    using Event = parent_::Type<T>;
    using Driver = this_::Driver;
    using Handler = this_::Handler<Event>;
    using Subscription = this_::Subscription;

    auto unsubscribe(Subscription const &) noexcept(false);
    template <class Handler> auto subscribe(Handler &&) noexcept(false);

protected:
    Type(Driver &driver) noexcept(true);

    Type(Type &&) = default;
    Type & operator = (Type &&) = default;

private:
    ::std::unique_ptr<Driver> driver_;

    Type() = delete;
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::event::source


#include "source+inl.hxx"
