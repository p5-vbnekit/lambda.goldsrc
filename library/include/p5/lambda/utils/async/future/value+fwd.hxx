#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::async::future {
namespace value {

namespace parent_ = this_;
namespace this_ = parent_::value;

namespace type {

namespace parent_ = this_;
namespace this_ = parent_::type;

using Default = void;

template <class = this_::Default> struct Wrapper;

} // namespace type

template <class ... T> using Type = typename this_::type::Wrapper<T ...>::Type;

} // namespace value

template <class ... T> using Value = this_::value::Type<T ...>;

} // namespace p5::lambda::utils::async::future
