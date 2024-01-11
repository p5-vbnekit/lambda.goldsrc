#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::sugar {
namespace not_object {

namespace parent_ = this_;
namespace this_ = parent_::not_object;

class Type;

} // namespace not_object

using NotObject = not_object::Type;

} // namespace p5::lambda::utils::sugar
