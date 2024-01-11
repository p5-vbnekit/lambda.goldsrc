#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::async::future {
namespace state {

namespace parent_ = this_;
namespace this_ = parent_::state;

enum class Type;

} // namespace state

using State = this_::state::Type;

} // namespace p5::lambda::utils::async::future
