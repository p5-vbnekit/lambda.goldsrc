#pragma once

#include <typeindex>
#include <type_traits>

#include "driver.hxx"


namespace p5::lambda::utils::container::driver::node {

template <class T> inline constexpr static auto key() noexcept(true) {
    static_assert(::std::is_class_v<T>);
    return ::std::type_index{typeid(::std::remove_cv_t<T>)};
}

namespace payload {

template <class T> inline consteval static auto qualifiers() noexcept(true) {
    static_assert(::std::is_class_v<T>);
    return this_::Qualifiers{(
        ::std::is_const_v<T> ? 0x1 : 0x00
    ) | (
        ::std::is_volatile_v<T> ? 0x2 : 0x00
    )};
}

} // namespace payload
} // namespace p5::lambda::utils::container::driver::node
