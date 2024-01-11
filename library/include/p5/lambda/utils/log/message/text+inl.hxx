#pragma once

#include <utility>

#include "text.hxx"


namespace p5::lambda::utils::log::message::text {
namespace view {
namespace policy {

template <class T> inline constexpr Line Traits<T>::line() noexcept(true) {
    if constexpr (requires () { T::line::value; }) return T::line::value;
    else if constexpr (requires() { T::line; }) return T::line;
    else return default_.line;
}

template <class T> inline constexpr bool
Traits<T>::terminate(T const &value) noexcept(true) {
    if constexpr (
        requires () { T::terminate::value; }
    ) return T::terminate::value;
    else if constexpr (
        requires() { value.terminate; }
    ) return value.terminate;
    else return default_.terminate;
}

} // namespace policy

namespace function {

template <class T> inline constexpr
auto this_::Type<T>::operator () () const noexcept(true) {
    return static_cast<parent_::parent_::Chunk const *>(nullptr);
}

} // namespace function
} // namespace view

template <class T> inline constexpr auto make(T &&driver) noexcept(true) {
    return this_::Type{::std::forward<T>(driver)};
}

} // namespace p5::lambda::utils::log::message::text
