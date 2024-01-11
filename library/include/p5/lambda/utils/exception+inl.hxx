#pragma once

#include <string>
#include <utility>
#include <exception>
#include <type_traits>

#include "generator.hxx"
#include "exception.hxx"


namespace p5::lambda::utils::exception {

template <class T> inline
::std::exception_ptr next(T &&exception) noexcept(true) requires(
    ::std::is_base_of_v<::std::exception, ::std::decay_t<T>>
) {
    try {
        ::std::rethrow_if_nested(::std::forward<T>(exception));
    } catch (...) { return ::std::current_exception(); }
    return {};
}

template <class T> inline auto iterate(T &&exception) noexcept(true) {
    return parent_::parent_::utils::generator::function::make([
        begin_ = true, value_ = [&exception] {
            if constexpr (::std::is_base_of_v<
                ::std::exception, ::std::decay_t<T>
            >) return ::std::make_exception_ptr(::std::forward<T>(exception));
            else return ::std::forward<T>(exception);
        } ()
    ] () mutable -> ::std::exception_ptr const * {
        if (value_) {
            if (begin_) { begin_ = false; return &value_; }
            if ((value_ = this_::next(value_))) return &value_;
        }
        return nullptr;
    });
}

inline auto iterate() noexcept(true) {
    return this_::iterate(::std::current_exception());
}

template <class T> inline
auto details(T &&exception, bool verbose) noexcept(false) requires(
    ::std::is_base_of_v<::std::exception, ::std::decay_t<T>>
) {
    return this_::details(::std::make_exception_ptr(
        ::std::forward<T>(exception)
    ), verbose);
}

} // namespace p5::lambda::utils::exception
