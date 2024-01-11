#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::async::subscription {
namespace driver {

namespace parent_ = this_;
namespace this_ = parent_::driver;

struct Type;

} // namespace driver

using Driver = this_::driver::Type;

} // namespace p5::lambda::utils::async::subscription
