#pragma once

#include "namespace_.hxx"

#include "container/type+fwd.hxx"
#include "container/lock+fwd.hxx"
#include "container/driver+fwd.hxx"
#include "container/pointer+fwd.hxx"


namespace p5::lambda::utils {
namespace container {

inline auto make() noexcept(false);

} // namespace container

using Container = container::Type;

} // namespace p5::lambda::utils
