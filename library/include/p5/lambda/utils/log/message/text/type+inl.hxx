#pragma once

#include <ranges>
#include <utility>

#include "../../../generator.hxx"

#include "type.hxx"
#include "driver.hxx"


namespace p5::lambda::utils::log::message::text {

inline auto * this_::Type::operator () () noexcept(true) {
    if (driver_) return ::std::exchange(driver_, nullptr);
    return driver_;
}

inline auto this_::Type::operator * () noexcept(true) {
#if ! (defined(__clang_major__) && (16 > __clang_major__))
    return ::std::views::all(
#else
    return (
#endif
        parent_::parent_::parent_::generator::function::make((*this)())
    );
}

inline constexpr this_::Type::Type(Driver *driver) noexcept(true):
    driver_(driver)
{}

inline constexpr this_::Type::Type(Driver &driver) noexcept(true):
    driver_(&driver)
{}

inline constexpr this_::Type::Type(Type &&other) noexcept(true):
    driver_{::std::exchange(other.driver_, nullptr)}
{}

inline constexpr auto & this_::Type::operator = (Type &&other) noexcept(true) {
    if (this != &other) ::std::swap(driver_, other.driver_);
    return *this;
}

} // namespace p5::lambda::utils::log::message::text
