#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::event {
namespace type {

namespace parent_ = this_;
namespace this_ = parent_::type;

using Default = void;

template <class = Default> struct Wrapper;

} // namespace type

template <class ... T> using Type = typename type::Wrapper<T ...>::Type;

} // namespace p5::lambda::utils::event
