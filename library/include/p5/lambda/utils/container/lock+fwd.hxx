#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::container {
namespace lock {

namespace parent_ = this_;
namespace this_ = parent_::lock;

template <class> struct Type;

} // namespace lock

template <class Interface> using Lock = this_::lock::Type<Interface>;

} // namespace p5::lambda::utils::container
