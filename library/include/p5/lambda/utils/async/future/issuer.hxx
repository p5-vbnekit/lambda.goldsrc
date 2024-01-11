#pragma once

#include <memory>
#include <type_traits>

#include "front.hxx"
#include "state.hxx"
#include "value.hxx"
#include "driver.hxx"
#include "handler.hxx"
#include "issuer+fwd.hxx"
#include "subscription.hxx"


namespace p5::lambda::utils::async::future::issuer {

using State = parent_::State;
using Driver = parent_::Driver;
using Subscription = parent_::Subscription;

template <class ... T> using Value = parent_::Value<T ...>;
template <class ... T> using Front = parent_::Front<this_::Value<T ...>>;
template <class ... T> using Handler = parent_::Handler<this_::Value<T ...>>;

struct Base {
    virtual ~Base() noexcept(true);

protected:
    ::std::shared_ptr<this_::Driver> driver_;

    Base() noexcept(false);
    Base(Base &&) noexcept(true);
    Base & operator = (Base &&) noexcept(true);

private:
    Base(Base const &) = delete;
    Base & operator = (Base const &) = delete;
};

template <class S, class D> inline consteval static
auto is_convertible() noexcept(true) {
    using Source_ = typename this_::Type<S>::Value;
    using Destination_ = typename this_::Type<S>::Value;
    if constexpr(::std::is_void_v<Source_>) return true;
    else return ::std::is_convertible_v<Destination_ *, Source_ *>;
}

template <> struct Type<void> final: this_::Base {
    using Front = this_::Front<void>;
    using State = this_::State;
    using Value = this_::Value<void>;
    using Driver = this_::Driver;
    using Handler = this_::Handler<void>;
    using Subscription = this_::Subscription;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto const & front() const noexcept(true);

    auto get() const noexcept(true);
    auto get(auto &&) const noexcept(false);
    auto set() noexcept(false);

    auto state() const noexcept(true);
    auto cancel() const noexcept(true);
    auto subscribe(auto &&) const noexcept(false);

    auto exception() const noexcept(true);
    auto exception(auto &&) noexcept(true);

    operator bool () const noexcept(true);
    operator Front () const noexcept(true);

    auto operator co_await() const noexcept(true);

    Type() noexcept(false);
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;
    virtual ~Type() noexcept(true) override = default;

private:
    Front front_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

template <class T> struct Type final: this_::Base {
    static_assert(! ::std::is_void_v<T>);

    using Front = this_::Front<T>;
    using State = this_::State;
    using Value = this_::Value<T>;
    using Driver = this_::Driver;
    using Handler = this_::Handler<T>;
    using Subscription = this_::Subscription;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto const & front() const noexcept(true);

    auto get() const noexcept(true);
    auto get(auto &&) const noexcept(false);
    auto set(auto &&) noexcept(false);
    auto emplace(auto && ...) noexcept(false) requires(
        ! ::std::is_reference_v<T>
    );

    auto state() const noexcept(true);
    auto cancel() const noexcept(true);
    auto subscribe(auto &&) const noexcept(false);

    auto exception() const noexcept(true);
    auto exception(auto &&) noexcept(true);

    operator bool () const noexcept(true);
    operator Front () const noexcept(true);

    auto & operator * () const noexcept(true);
    auto operator co_await() const noexcept(true);

    Type() noexcept(false);

    template <class O> explicit Type(
        this_::Type<O> &&
    ) noexcept(true) requires(
        this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
    );

    template <class O> auto & operator = (
        this_::Type<O> &&
    ) noexcept(true) requires(
        this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
    );

    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;
    virtual ~Type() noexcept(true) override = default;

private:
    Front front_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::async::future::issuer


#include "issuer+inl.hxx"
