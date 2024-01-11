#pragma once

#include <string>
#include <exception>
#include <type_traits>

#include "namespace_.hxx"


namespace p5::lambda::utils::exception {

namespace parent_ = this_;
namespace this_ = parent_::exception;

::std::exception_ptr next(
    ::std::exception_ptr const & = ::std::current_exception()
) noexcept(true);

template <class T> ::std::exception_ptr next(T &&) noexcept(true) requires(
    ::std::is_base_of_v<::std::exception, ::std::decay_t<T>>
);

auto iterate() noexcept(true);
template <class T> auto iterate(T &&) noexcept(true);

::std::string details(
    ::std::exception_ptr const & = ::std::current_exception(), bool = true
) noexcept(false);

template <class T> auto details(T &&, bool = true) noexcept(false) requires(
    ::std::is_base_of_v<::std::exception, ::std::decay_t<T>>
);

} // namespace p5::lambda::utils::exception
