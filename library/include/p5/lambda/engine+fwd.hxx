#pragma once

#include "namespace_.hxx"


namespace p5::lambda::engine {

namespace parent_ = this_;
namespace this_ = parent_::engine;

struct Globals;
struct Functions;

namespace api {

namespace parent_ = this_;
namespace this_ = parent_::api;

} // namespace api
} // namespace p5::lambda::engine
