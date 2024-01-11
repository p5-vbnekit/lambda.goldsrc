#pragma once

#include <utility>
#include <type_traits>

#include "type.hxx"
#include "interface.hxx"


namespace p5::lambda::utils::log {

template <class T> inline
auto this_::Type::operator >> (T &&handler) noexcept(false) requires(
    ::std::is_convertible_v<T, Handler>
) {
    this->handler = ::std::forward<T>(handler);
}

template <class T> this_::Type::Type(T &&handler) noexcept(false) requires(
    ! ::std::is_base_of_v<Interface, ::std::decay_t<T>>
): handler(::std::forward<T>(handler)) {}

} // namespace p5::lambda::utils::log
