#pragma once

#include <utility>
#include <stdexcept>
#include <type_traits>

#include "type.hxx"


namespace p5::lambda::utils::async::task {

template <class T> inline auto this_::Type<T>::unlink() noexcept(true) {
    auto coroutine_ = ::std::exchange(this->coroutine_, nullptr);
    if (! coroutine_) return false;
    auto const future_ = ::std::move(this->future_);
    auto &state_ = coroutine_.promise().state_;
    if (1 < state_.references) {
        if constexpr (::std::is_volatile_v<
            ::std::remove_reference_t<decltype(state_.references)>
        >) state_.references = state_.references - 1;
        else --(state_.references);
        return true;
    }
    future_.cancel();
    if (state_.suspended) coroutine_.destroy();
    else state_.references = 0;
    return true;
}

template <class T> inline auto this_::Type<T>::linked() const noexcept(true) {
    return static_cast<bool>(coroutine_);
}

template <class T> inline auto this_::Type<T>::state() const noexcept(true) {
    using Phase_ = ::std::decay_t<decltype(coroutine_.promise().state_.phase)>;
    using FutureState_ = ::std::decay_t<decltype(future_.state())>;
    if (coroutine_) switch (coroutine_.promise().state_.phase) {
    default: break;
    case Phase_::Initial:
        switch (future_.state()) {
        default: break;
        case FutureState_::Initial: return this_::State::Initial;
        case FutureState_::Cancelled: return this_::State::Cancelled;
        }
        break;
    case Phase_::Started:
        switch (future_.state()) {
        default: break;
        case FutureState_::Initial: return this_::State::Started;
        case FutureState_::Ready: return this_::State::Finished;
        case FutureState_::Cancelled: return this_::State::Cancelled;
        }
        break;
    case Phase_::Finished:
        switch (future_.state()) {
        default: break;
        case FutureState_::Ready: return this_::State::Finished;
        case FutureState_::Cancelled: return this_::State::Cancelled;
        }
        break;
    }
    return this_::State::Bad;
}

template <class T> inline
auto const & this_::Type<T>::future() const noexcept(true) {
    return future_;
}

template <class T> inline auto this_::Type<T>::stop() const noexcept(true) {
    return future_.cancel();
}

template <class T> inline auto this_::Type<T>::start() const noexcept(true) {
    if (! coroutine_) return false;
    auto const &state_ = coroutine_.promise().state_;
    if (! state_.suspended) return false;
    if (::std::decay_t<
        decltype(state_.phase)
    >::Initial != state_.phase) return false;
    if (! state_.future) return false;
    if (future_) return false;
    coroutine_.resume();
    return true;
}

template <class T> inline this_::Type<T>::Type(
    ::std::decay_t<decltype(coroutine_)> const &coroutine
) noexcept(false): future_{}, coroutine_{coroutine} {
    using Bad_ = ::std::invalid_argument;
    if (! coroutine) throw Bad_{"bad coroutine"};
    if (coroutine.done()) throw Bad_{"bad coroutine"};
    auto &state_ = coroutine.promise().state_;
    if (::std::decay_t<
        decltype(state_.phase)
    >::Initial != state_.phase) throw Bad_{"bad coroutine"};
    if (0 != state_.references) throw Bad_{"bad coroutine"};
    if (state_.future) future_ = state_.future->front();
    state_.references = 1;
}

template <class T> inline this_::Type<T>::Type(Type &&other) noexcept(true):
    future_{::std::move(other.future_)},
    coroutine_{::std::exchange(other.coroutine_, nullptr)}
{}

template <class T> inline
this_::Type<T>::Type(Type const &other) noexcept(true):
    future_{other.future_}, coroutine_{other.coroutine_}
{
    if (coroutine_) {
        auto &references_ = coroutine_.promise().state_.references;
        if constexpr (::std::is_volatile_v<
            ::std::remove_reference_t<decltype(references_)>
        >) references_ = 1 + references_;
        else ++references_;
    }
}

template <class T> inline
auto & this_::Type<T>::operator = (Type &&other) noexcept(true) {
    if (this != &other) {
        unlink();
        future_ = ::std::move(other.future_);
        coroutine_ = ::std::exchange(other.coroutine_, nullptr);
    }
    return *this;
}

template <class T> inline
auto & this_::Type<T>::operator = (Type const &other) noexcept(true) {
    if (this != &other) {
        unlink();
        future_ = other.future_;
        coroutine_ = other.coroutine_;
        if (coroutine_) {
            auto &references_ = coroutine_.promise().state_.references;
            if constexpr (::std::is_volatile_v<
                ::std::remove_reference_t<decltype(references_)>
            >) references_ = 1 + references_;
            else ++references_;
        }
    }
    return *this;
}

template <class T> inline
this_::Type<T>::Type::~Type() noexcept(true) { unlink(); }

} // namespace p5::lambda::utils::async::task
