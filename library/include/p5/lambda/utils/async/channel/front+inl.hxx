#pragma once

#include <utility>
#include <stdexcept>
#include <type_traits>

#include "front.hxx"
#include "exceptions.hxx"


namespace p5::lambda::utils::async::channel::front {

template <class T> inline auto this_::Type<T>::unlink() noexcept(true) {
    auto &driver_ = this_::Base::driver_;
    if (driver_.expired()) return false;
    driver_.reset();
    return true;
}

template <class T> inline auto this_::Type<T>::linked() const noexcept(true) {
    return ! this_::Base::driver_.expired();
}

template <class T> inline
auto this_::Type<T>::subscribe(auto &&handler) const noexcept(false) {
    auto const driver_ = this_::Base::driver_.lock();
    if (! driver_) throw parent_::exceptions::BadState{
        "bad channel state: empty driver pointer"
    };
    auto &&handler_ = Handler{::std::forward<decltype(handler)>(handler)};
    if (! handler_) throw ::std::invalid_argument{"empty handler"};
    if constexpr (::std::is_void_v<Event>) return driver_->subscribe([
        handler_ = ::std::move(handler_)
    ] (auto const *) { handler_(); });
    else if constexpr (::std::is_convertible_v<
        Handler, typename ::std::decay_t<decltype(*driver_)>::Handler
    >) return driver_->subscribe(::std::move(handler_));
    else return driver_->subscribe([
        handler_ = ::std::move(handler_)
    ] (auto const *event) { handler_(*static_cast<Event const *>(event)); });
}

template <class T> inline this_::Type<T>::Type(
    ::std::decay_t<decltype(driver_)> &&driver
) noexcept(true): this_::Base{::std::move(driver)} {}

template <class T> template <class O> inline this_::Type<T>::Type(
    this_::Type<O> &&other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
): this_::Base{::std::move(static_cast<this_::Base &>(other))} {}

template <class T> template <class O> inline this_::Type<T>::Type(
    this_::Type<O> const &other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
): this_::Base{other} {}

template <class T> template <class O> inline auto & this_::Type<T>::operator = (
    this_::Type<O> &&other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
) {
    static_cast<this_::Base *>(this)->operator = (::std::move(other));
    return *this;
}

template <class T> template <class O> inline auto & this_::Type<T>::operator = (
    this_::Type<O> const &other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
) {
    static_cast<this_::Base *>(this)->operator = (other);
    return *this;
}

} // namespace p5::lambda::utils::async::channel::front
