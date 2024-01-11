#pragma once

#include "container.hxx"


namespace p5::lambda::utils::container {

inline auto make() noexcept(false) { return this_::Type{}; }

} // namespace p5::lambda::utils::container
