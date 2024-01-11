#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log::message::text {
namespace driver {

namespace parent_ = this_;
namespace this_ = parent_::driver;

namespace interface {

namespace parent_ = this_;
namespace this_ = parent_::interface;

struct Type;

} // namespace interface

using Interface = interface::Type;

template <class> struct Type;

template <class T> auto make(T &&) noexcept(false);

} // namespace driver

template <class T> using Driver = driver::Type<T>;

} // namespace p5::lambda::utils::log::message::text
