#pragma once

#include <functional>

#include "namespace_.hxx"

#include "message+fwd.hxx"


namespace p5::lambda::utils::log {
namespace handler {

namespace parent_ = this_;
namespace this_ = parent_::handler;

using Type = ::std::function<void(Message &&)>;
using Message = parent_::Message;

template <class T> auto capture(T &&) noexcept(false);

} // namespace handler

using Handler = handler::Type;

} // namespace p5::lambda::utils::log
