#pragma once

#include "namespace_.hxx"


namespace p5::lambda::game {

namespace parent_ = this_;
namespace this_ = parent_::game;

struct Functions;

namespace functions {

namespace parent_ = this_;
namespace this_ = parent_::functions;

struct Pointers;
struct Standard;
struct Extension;

} // namespace functions

namespace api {

namespace parent_ = this_;
namespace this_ = parent_::api;

template <class> consteval auto version() noexcept(true);
consteval void version(void const *) noexcept(true) = delete;

} // namespace api
} // namespace p5::lambda::game
