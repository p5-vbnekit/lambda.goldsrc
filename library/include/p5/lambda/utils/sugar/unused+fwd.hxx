#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::sugar {

template <class ... T> constexpr auto unused(T && ...) noexcept(true);

} // namespace p5::lambda::utils::sugar
