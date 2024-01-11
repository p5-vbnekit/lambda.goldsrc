#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::event {
namespace dispatcher {

namespace parent_ = this_;
namespace this_ = parent_::dispatcher;

template <class = void> struct Type;

namespace driver_ {

namespace parent_ = this_;
namespace this_ = parent_::driver_;

struct Type;

} // namespace driver_

using Driver_ = driver_::Type;

} // namespace dispatcher

template <class ... T> using Dispatcher = dispatcher::Type<T ...>;

} // namespace p5::lambda::utils::event
