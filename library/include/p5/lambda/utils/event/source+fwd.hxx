#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::event {
namespace source {

namespace parent_ = this_;
namespace this_ = parent_::source;

namespace base {

namespace parent_ = this_;
namespace this_ = parent_::base;

struct Type;

} // namespace base

using Base = base::Type;

namespace driver {

namespace parent_ = this_;
namespace this_ = parent_::driver;

struct Type;

} // namespace driver

using Driver = driver::Type;

template <class = void> struct Type;

} // namespace source

template <class ... T> using Source = source::Type<T ...>;

} // namespace p5::lambda::utils::event
