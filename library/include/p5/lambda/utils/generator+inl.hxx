#pragma once

#include <utility>
#include <variant>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <type_traits>

#include "generator.hxx"


namespace p5::lambda::utils::generator {
namespace function {
namespace machine {

template <class T> inline constexpr
auto & this_::Type<T>::reference_() noexcept(false) {
    if constexpr (::std::is_pointer_v<Driver>) return *driver_;
    else return driver_;
}

template <class T> inline constexpr
this_::Type<T>::operator bool () noexcept(true) {
    if constexpr (
        ::std::is_pointer_v<Driver>
    ) if (! this->driver_) return false;
    auto &driver_ = reference_();
    if (0 == state_.index()) {
        try { state_.template emplace<1>(driver_()); }
        catch (...) { state_.template emplace<2>(::std::current_exception()); }
    }
    if (1 != state_.index()) return false;
    return nullptr != ::std::get<Pointer>(state_);
}

template <class T> inline constexpr
auto & this_::Type<T>::operator * () noexcept(false) {
    if constexpr (
        ::std::is_pointer_v<Driver>
    ) if (! this->driver_) throw ::std::logic_error{"invalid state"};
    auto &driver_ = reference_();
    switch(state_.index()) {
    default: throw ::std::logic_error{"invalid state"};
    case 0:
        if constexpr (true) {
            try { state_.template emplace<1>(driver_()); }
            catch (...) {
                state_.template emplace<2>(::std::current_exception());
                throw;
            }
        }
        if (1 != state_.index()) throw ::std::logic_error{"invalid state"};
    case 1: break;
    case 2:
        if constexpr (true) {
            auto const &exception_ = ::std::get<::std::exception_ptr>(state_);
            if (exception_) ::std::rethrow_exception(exception_);
            throw ::std::logic_error{"invalid state"};
        }; break;
    }
    auto * const result_ = ::std::get<Pointer>(state_);
    if (! result_) throw ::std::logic_error{"invalid state"};
    return *result_;
}

template <class T> inline constexpr
auto this_::Type<T>::operator () () noexcept(true) {
    if constexpr (
        ::std::is_pointer_v<Driver>
    ) if (! this->driver_) return false;
    auto &driver_ = reference_();
    if (0 == state_.index()) {
        try { state_.template emplace<1>(driver_()); }
        catch (...) {
            state_.template emplace<2>(::std::current_exception());
            return false;
        }
    }
    if (1 != state_.index()) return false;
    try {
        if (! ::std::get<Value *>(state_)) return false;
        state_.template emplace<1>(driver_());
    }
    catch (...) {
        state_.template emplace<2>(::std::current_exception());
        return false;
    }
    return true;
}

template <class T> inline constexpr
this_::Type<T>::Type() noexcept(true): driver_{[] {
    if constexpr (::std::is_pointer_v<Driver>) return nullptr;
    else return Driver{};
} ()} {}

template <class T> inline constexpr
this_::Type<T>::Type(Type &&other) noexcept(true):
    state_{::std::exchange(other.state_, {})},
    driver_{::std::move(other.driver_)}
{
    if constexpr (::std::is_pointer_v<T>) other.driver_ = nullptr;
}

template <class T> inline constexpr
auto & this_::Type<T>::operator = (Type &&other) noexcept(true) {
    if (&other != this) {
        ::std::swap(other.state_, state_);
        ::std::swap(other.driver_, driver_);
    }
    return *this;
}

} // namespace machine

namespace iterator {

template <class T> inline constexpr
this_::Type<T> & this_::Type<T>::operator ++ () noexcept(true) {
    if (machine_) (*machine_)();
    return *this;
}

#if defined(__GNUC__) && (! defined __clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#endif
template <class T> inline constexpr
void this_::Type<T>::operator ++ (int) noexcept(true) { ++(*this); }
#if defined(__GNUC__) && (! defined __clang__)
#pragma GCC diagnostic pop
#endif

template <class T> inline constexpr
auto & this_::Type<T>::operator * () const noexcept(false) {
    if (! machine_) throw ::std::logic_error{"invalid state"};
    return **machine_;
}

template <class T> inline constexpr bool this_::Type<T>::operator == (
    ::std::default_sentinel_t const &
) const noexcept(true) { return ! (machine_ && *machine_); }

template <class T> inline constexpr this_::Type<T>::Type(
    ::std::decay_t<decltype(*machine_)> &machine
) noexcept(true): machine_(&machine) {}

} // namespace iterator

template <class T> inline constexpr
auto this_::Type<T>::end() const noexcept(true) {
    return ::std::default_sentinel;
}

template <class T> inline constexpr
auto this_::Type<T>::begin() noexcept(true) {
    using Iterator_ = this_::Iterator<
        typename ::std::decay_t<decltype(machine_)>::Driver
    >;
    if (static_cast<bool>(machine_)) return Iterator_{machine_};
    return Iterator_{};
}

template <class Driver> template <class T> inline constexpr
this_::Type<Driver>::Type(T &&driver) noexcept(true) :
    machine_(::std::forward<T>(driver))
{}

template <class T> constexpr auto make(T &&driver) noexcept(true) {
    return this_::Type<::std::decay_t<T>>{::std::forward<T>(driver)};
}

} // namespace function

namespace coroutine {
namespace promise {

template <class T> inline constexpr
auto this_::Type<T>::get_return_object() noexcept(true) {
    return parent_::Type<T>{Handle_<T>::from_promise(*this)};
}

template <class T> inline constexpr
auto this_::Type<T>::initial_suspend() noexcept(true) {
    return ::std::suspend_always{};
}

template <class T> inline constexpr
auto this_::Type<T>::yield_value(T &value) noexcept(true) {
    payload_.template emplace<1>(&value);
    return ::std::suspend_always{};
}

template <class T> inline constexpr
auto this_::Type<T>::yield_value(T &&value) noexcept(true) {
    return yield_value(value);
}

template <class T> inline constexpr
auto this_::Type<T>::return_void() noexcept(true) {}

template <class T> inline constexpr
auto this_::Type<T>::unhandled_exception() noexcept(true) {
    payload_.template emplace<2>(::std::current_exception());
}

template <class T> inline constexpr
auto this_::Type<T>::final_suspend() noexcept(true) {
    return ::std::suspend_always{};
}

} // namespace promise

namespace iterator {

template <class T> inline constexpr
this_::Type<T> & this_::Type<T>::operator ++ () noexcept(true) {
    if (1 == coroutine_.promise().payload_.index()) coroutine_.resume();
    return *this;
}

#if defined(__GNUC__) && (! defined __clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#endif
template <class T> inline constexpr
void this_::Type<T>::operator ++ (int) noexcept(true) { ++(*this); }
#if defined(__GNUC__) && (! defined __clang__)
#pragma GCC diagnostic pop
#endif

template <class T> inline constexpr auto &
this_::Type<T>::operator * () const noexcept(false) {
    auto const &payload_ = coroutine_.promise().payload_;
    if (1 != payload_.index()) {
        if (2 == payload_.index()) {
            auto const &exception_ = ::std::get<2>(payload_);
            if (exception_) ::std::rethrow_exception(exception_);
        }
        throw ::std::logic_error{"invalid state"};
    }
    auto const pointer_ = ::std::get<1>(payload_);
    if (! pointer_) throw ::std::logic_error{"invalid state"};
    return *pointer_;
}

template <class T> inline constexpr bool
this_::Type<T>::operator == (
    ::std::default_sentinel_t const &
) const noexcept(true) {
    if (! coroutine_) return true;
    if (coroutine_.done()) return true;
    auto const &payload_ = coroutine_.promise().payload_;
    switch (payload_.index()) {
    default: break;
    case 1:
        if constexpr (true) {
            auto const &pointer_ = ::std::get<1>(payload_);
            if (static_cast<bool>(pointer_)) return false;
        }; break;
    case 2:
        if constexpr (true) {
            auto const &pointer_ = ::std::get<2>(payload_);
            if (static_cast<bool>(pointer_)) return false;
        }; break;
    }
    return true;
}

template <class T> inline constexpr
this_::Type<T>::Type(parent_::Handle_<T> const &coroutine) noexcept(true):
    coroutine_{coroutine}
{
    if (! coroutine_) return;
    if (coroutine_.done()) return;
    auto &payload_ = coroutine_.promise().payload_;
    if (0 != payload_.index()) return;
    payload_.template emplace<1>(nullptr);
    coroutine_.resume();
}

} // namespace iterator

template <class T> inline constexpr
auto this_::Type<T>::end() const noexcept(true) {
    return ::std::default_sentinel_t{};
}

template <class T> inline constexpr
auto this_::Type<T>::begin() const noexcept(true) {
    return this_::Iterator<T>{coroutine_};
}

template <class T> inline constexpr this_::Type<T>::Type(
    this_::Handle_<T> const &coroutine
) noexcept(true): coroutine_{coroutine} {}

template <class T> inline this_::Type<T>::Type::~Type() noexcept(true) {
    if (coroutine_) coroutine_.destroy();
}

template <class T> inline constexpr
this_::Type<T>::Type(Type &&other) noexcept(true):
    coroutine_{::std::exchange(other.coroutine_, nullptr)}
{}

template <class T> inline constexpr
auto & this_::Type<T>::operator = (Type &&other) noexcept(true) {
    if (&other != this) ::std::swap(other.coroutine_, coroutine_);
    return *this;
}

template <class T> inline constexpr auto from_range(T &&source) {
    return [] (auto source) -> this_::Type<
        ::std::remove_reference_t<decltype(*::std::begin(source))>
    > {
        for (auto &value_: source) co_yield value_;
    } (::std::forward<T>(source));
}

} // namespace coroutine
} // namespace p5::lambda::utils::generator
