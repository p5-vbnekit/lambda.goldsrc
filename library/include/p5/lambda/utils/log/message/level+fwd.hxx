#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log::message {
namespace level {

namespace parent_ = this_;
namespace this_ = parent_::level;

enum class Type;

template <Type> constexpr auto const * to_string() noexcept(true);
auto const * to_string(Type const &) noexcept(false);

} // namespace level

using Level = level::Type;

} // namespace p5::lambda::utils::log::message
