#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::sugar {
namespace uncopyable {

namespace parent_ = this_;
namespace this_ = parent_::uncopyable;

class Type;

} // namespace uncopyable

using Uncopyable = uncopyable::Type;

} // namespace p5::lambda::utils::sugar
