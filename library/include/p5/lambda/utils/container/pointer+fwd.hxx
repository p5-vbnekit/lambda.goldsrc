#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::container {
namespace pointer {

namespace parent_ = this_;
namespace this_ = parent_::pointer;

namespace driver {

namespace parent_ = this_;
namespace this_ = parent_::driver;

struct Type;

} // namespace driver

using Driver = driver::Type;

template <class> struct Type;

} // namespace pointer

template <class Interface> using Pointer = pointer::Type<Interface>;

} // namespace p5::lambda::utils::container
