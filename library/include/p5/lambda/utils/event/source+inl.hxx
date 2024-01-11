#pragma once

#include <memory>
#include <stdexcept>
#include <type_traits>

#include "source+fwd.hxx"
#include "subscription+fwd.hxx"

#include "type.hxx"
#include "source.hxx"
#include "handler.hxx"


namespace p5::lambda::utils::event::source {
namespace driver {

using Event = void const *;
using Handler = parent_::Handler<Event>;
using Subscription = parent_::Subscription;

struct Type {
    using Handler = this_::Handler;
    using Subscription = this_::Subscription;

    virtual Subscription subscribe(Handler &&) noexcept(false) = 0;
    virtual void unsubscribe(Subscription const &) noexcept(false) = 0;

    virtual ~Type() noexcept(true);
};

} // namespace driver

using Driver = driver::Type;


template <class T> inline auto this_::Type<T>::unsubscribe(
    Subscription const &subscription
) noexcept(false) { driver_->unsubscribe(subscription); }

template <class T> template <class H> inline
auto this_::Type<T>::subscribe(H &&handler) noexcept(false) {
    auto &&handler_ = [&handler] () -> decltype(auto) {
        if constexpr (
            ::std::is_same_v<Handler, ::std::decay_t<H>>
        ) return ::std::forward<H>(handler);
        else return Handler{::std::forward<H>(handler)};
    } ();
    if (! handler_) throw ::std::invalid_argument{"empty handler"};
    return driver_->subscribe([
        handler_ = ::std::move(handler_)
    ] (auto const &event) { try {
        if constexpr (::std::is_same_v<T, void>) handler_();
        else handler_(*static_cast<Event const *>(event));
    } catch (...) {}});
}

template <class T> inline
this_::Type<T>::Type(Driver &driver) noexcept(true): driver_{&driver} {}

} // namespace p5::lambda::utils::event::source
