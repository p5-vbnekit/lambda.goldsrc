#pragma once

#include <memory>
#include <type_traits>

#include "front.hxx"
#include "event.hxx"
#include "driver.hxx"
#include "issuer+fwd.hxx"
#include "subscription.hxx"


namespace p5::lambda::utils::async::channel::issuer {

using Driver = parent_::Driver;
using Subscription = parent_::Subscription;

template <class ... T> using Front = parent_::Front<T ...>;
template <class ... T> using Event = parent_::Event<T ...>;
template <class ... T> using Handler = parent_::Handler<T ...>;

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
    using Source_ = typename this_::Type<S>::Event;
    using Destination_ = typename this_::Type<S>::Event;
    if constexpr(::std::is_void_v<Source_>) return true;
    else return ::std::is_convertible_v<Destination_ *, Source_ *>;
}

template <> struct Type<void> final: this_::Base {
    using Front = this_::Front<void>;
    using Event = this_::Event<void>;
    using Driver = this_::Driver;
    using Handler = this_::Handler<void>;
    using Subscription = this_::Subscription;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto const & front() const noexcept(true);

    auto dispatch() noexcept(true);
    auto subscribe(auto &&) const noexcept(false);

    operator Front () const noexcept(true);

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
    using Event = this_::Event<T>;
    using Driver = this_::Driver;
    using Handler = this_::Handler<T>;
    using Subscription = this_::Subscription;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto const & front() const noexcept(true);

    auto dispatch(Event const &) const noexcept(true);

#if (! defined(__clang_major__)) || (18 != __clang_major__)
    auto dispatch(auto &&event) const noexcept(
        ::std::is_nothrow_constructible_v<Event, decltype(event)>
    ) requires(! ::std::is_base_of_v<
        this_::Event<T>, ::std::decay_t<decltype(event)> // clang-18 crash reason
    >);
#endif

    auto subscribe(auto &&) const noexcept(false);

    operator Front () const noexcept(true);

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

} // namespace p5::lambda::utils::async::channel::issuer


#include "issuer+inl.hxx"
