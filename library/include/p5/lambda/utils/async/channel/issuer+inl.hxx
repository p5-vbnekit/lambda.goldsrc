#pragma once

#include <utility>
#include <type_traits>

#include "issuer.hxx"


namespace p5::lambda::utils::async::channel::issuer {

inline auto this_::Type<void>::unlink() noexcept(true) {
    auto &driver_ = this_::Base::driver_;
    if (! driver_) return false;
    driver_.reset();
    return true;
}

inline auto this_::Type<void>::linked() const noexcept(true) {
    return static_cast<bool>(this_::Base::driver_);
}

inline auto const & this_::Type<void>::front() const noexcept(true) {
    return front_;
}

inline auto this_::Type<void>::dispatch() noexcept(true) {
    auto const &driver_ = this_::Base::driver_;
    return driver_ && driver_->dispatch(nullptr);
}

inline auto this_::Type<void>::subscribe(auto &&handler) const noexcept(false) {
    return front_.subscribe(::std::forward<decltype(handler)>(handler));
}

inline this_::Type<void>::operator this_::Front<void> () const noexcept(true) {
    return front_;
}

inline this_::Type<void>::Type() noexcept(false):
    this_::Base{}, front_{this_::Base::driver_}
{}

template <class T> inline auto this_::Type<T>::unlink() noexcept(true) {
    auto &driver_ = this_::Base::driver_;
    if (! driver_) return false;
    driver_.reset();
    return true;
}

template <class T> inline auto this_::Type<T>::linked() const noexcept(true) {
    return static_cast<bool>(this_::Base::driver_);
}

template <class T> inline
auto const & this_::Type<T>::front() const noexcept(true) {
    return front_;
}

template <class T> inline
auto this_::Type<T>::dispatch(Event const &event) const noexcept(true) {
    auto const &driver_ = this_::Base::driver_;
    return driver_ && driver_->dispatch(&event);
}

template <class T> inline
auto this_::Type<T>::dispatch(auto &&event) const noexcept(
    ::std::is_nothrow_constructible_v<Event, decltype(event)>
) requires(! ::std::is_base_of_v<
    this_::Event<T>, ::std::decay_t<decltype(event)>
>) {
    auto const &driver_ = this_::Base::driver_;
    if (! driver_) return false;
    auto const event_ = Event{::std::forward<decltype(event)>(event)};
    return driver_->dispatch(&event_);
}

template <class T> inline
auto this_::Type<T>::subscribe(auto &&handler) const noexcept(false) {
    return front_.subscribe(::std::forward<decltype(handler)>(handler));
}

template <class T> inline
this_::Type<T>::operator this_::Front<T> () const noexcept(true) {
    return front_;
}

template <class T> inline this_::Type<T>::Type() noexcept(false):
    this_::Base{}, front_{this_::Base::driver_}
{}

template <class T> template <class O> inline this_::Type<T>::Type(
    this_::Type<O> &&other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
):
    this_::Base{::std::move(static_cast<this_::Base &>(other))},
    front_{this_::Base::driver_}
{}

template <class T> template <class O> inline auto & this_::Type<T>::operator = (
    this_::Type<O> &&other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
) {
    static_cast<this_::Base *>(this)->operator = (::std::exchange(
        other, ::std::decay_t<decltype(other)>{this_::Base::driver_}
    ));
    front_ = ::std::decay_t<decltype(front_)>{this_::Base::driver_};
    return *this;
}

} // namespace p5::lambda::utils::async::channel::issuer
