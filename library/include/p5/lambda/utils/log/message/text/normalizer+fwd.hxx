#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log::message::text {
namespace normalizer {

namespace parent_ = this_;
namespace this_ = parent_::normalizer;

namespace driver {

namespace parent_ = this_;
namespace this_ = parent_::driver;

template <class> struct Type;

template <class T> constexpr auto make(T &&) noexcept(true);

} // namespace driver

template <class T> using Driver = driver::Type<T>;

template <class T> constexpr auto make(T &&) noexcept(true);

} // namespace normalizer
} // namespace p5::lambda::utils::log::message::text
