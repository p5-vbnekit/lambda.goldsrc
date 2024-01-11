#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::container {
namespace pointer {

namespace parent_ = this_;
namespace this_ = parent_::pointer;

template <class> struct Type;

} // namespace pointer

template <class Interface> using Pointer = this_::pointer::Type<Interface>;

} // namespace p5::lambda::utils::container
