#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log::message {
namespace location {

namespace parent_ = this_;
namespace this_ = parent_::location;

namespace native {

namespace parent_ = this_;
namespace this_ = parent_::native;

} // namespace native

struct Type;

constexpr auto unknown() noexcept(true);
constexpr auto unknown(Type const &) noexcept(true);

template <class ... T> auto to_string(
    Type const &, T && ...
) noexcept(false);

auto inspect() noexcept(true);

template <class ... T> constexpr auto make(T && ...) noexcept(true);

} // namespace location

using Location = location::Type;

} // namespace p5::lambda::utils::log::message
