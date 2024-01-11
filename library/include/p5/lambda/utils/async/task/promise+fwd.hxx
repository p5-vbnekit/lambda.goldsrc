#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::async::task {
namespace promise {

namespace parent_ = this_;
namespace this_ = parent_::promise;

template <class = void> struct Type;

} // namespace promise

template <class ... T> using Promise = this_::promise::Type<T ...>;

} // namespace p5::lambda::utils::async::task
