#pragma once

#include "event+fwd.hxx"
#include "namespace_.hxx"


namespace p5::lambda::utils::async::channel {
namespace front {

namespace parent_ = this_;
namespace this_ = parent_::front;

template <class = parent_::event::type::Default> struct Type;

} // namespace front

template <class ... T> using Front = this_::front::Type<T ...>;

} // namespace p5::lambda::utils::async::channel
