#pragma once

#include "unused.hxx"


namespace p5::lambda::utils::sugar {

template <class ... T> inline constexpr auto unused(T && ...) noexcept(true) {}

} // namespace p5::lambda::utils::sugar
