#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::container {
namespace driver {

namespace parent_ = this_;
namespace this_ = parent_::driver;

namespace node {

namespace parent_ = this_;
namespace this_ = parent_::node;

struct Type;

} // namespace node

using Node = this_::node::Type;

struct Type;

} // namespace driver

using Driver = this_::driver::Type;

} // namespace p5::lambda::utils::container
