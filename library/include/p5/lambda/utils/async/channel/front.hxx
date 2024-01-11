#pragma once

#include <memory>
#include <type_traits>

#include "event.hxx"
#include "driver.hxx"
#include "handler.hxx"
#include "front+fwd.hxx"
#include "subscription.hxx"


namespace p5::lambda::utils::async::channel::front {

using Driver = parent_::Driver;
using Subscription = parent_::Subscription;

template <class ... T> using Event = parent_::Event<T ...>;
template <class ... T> using Handler = parent_::Handler<T ...>;

struct Base {
    virtual ~Base() noexcept(true);

protected:
    ::std::weak_ptr<Driver> driver_;

    Base(Base &&) noexcept(true);
    Base(Base const &) noexcept(true);

    Base & operator = (Base &&) noexcept(true);
    Base & operator = (Base const &) noexcept(true);

    explicit Base(::std::decay_t<decltype(driver_)> && = {}) noexcept(true);
};

template <class S, class D> inline consteval static
auto is_convertible() noexcept(true) {
    using Source_ = typename this_::Type<S>::Event;
    using Destination_ = typename this_::Type<S>::Event;
    if constexpr(::std::is_void_v<Destination_>) return true;
    else return ::std::is_convertible_v<Source_ *, Destination_ *>;
}

template <class T> struct Type final: this_::Base {
    using Event = this_::Event<T>;
    using Driver = this_::Driver;
    using Handler = this_::Handler<T>;
    using Subscription = this_::Subscription;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto subscribe(auto &&) const noexcept(false);

    explicit Type(::std::decay_t<decltype(driver_)> && = {}) noexcept(true);

    template <class O> explicit Type(
        this_::Type<O> &&
    ) noexcept(true) requires(
        this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
    );

    template <class O> explicit Type(
        this_::Type<O> const &
    ) noexcept(true) requires(
        this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
    );

    template <class O> auto & operator = (
        this_::Type<O> &&
    ) noexcept(true) requires(
        this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
    );

    template <class O> auto & operator = (
        this_::Type<O> const &
    ) noexcept(true) requires(
        this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
    );

    Type(Type &&) noexcept(true) = default;
    Type(Type const &) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;
    Type & operator = (Type const &) noexcept(true) = default;
    virtual ~Type() noexcept(true) override = default;
};

} // namespace p5::lambda::utils::async::channel::front


#include "front+inl.hxx"
