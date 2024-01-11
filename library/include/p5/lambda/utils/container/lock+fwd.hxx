#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::container {
namespace lock {

namespace parent_ = this_;
namespace this_ = parent_::lock;

namespace driver {

namespace parent_ = this_;
namespace this_ = parent_::driver;

struct Type;

} // namespace driver

using Driver = driver::Type;

template <class> struct Type;

} // namespace lock

template <class Interface> using Lock = lock::Type<Interface>;

} // namespace p5::lambda::utils::container
