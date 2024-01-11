#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log::message::text {
namespace chunk {

namespace parent_ = this_;
namespace this_ = parent_::chunk;

struct Type;

template <class ... T> constexpr auto make(T && ...) noexcept(true) = delete;

} // namespace chunk

using Chunk = chunk::Type;

} // namespace p5::lambda::utils::log::message::text
