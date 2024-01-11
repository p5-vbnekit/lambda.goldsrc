#pragma once

#include "namespace_.hxx"

#include "type+fwd.hxx"


namespace p5::lambda::utils::event {
namespace handler {

namespace parent_ = this_;
namespace this_ = parent_::handler;

namespace type {

namespace parent_ = this_;
namespace this_ = parent_::type;

template <class = parent_::parent_::type::Default> struct Wrapper;

} // namespace type

template <class ... T> using Type = typename type::Wrapper<T ...>::Type;

} // namespace handler

template <class ... T> using Handler = handler::Type<T ...>;

} // namespace p5::lambda::utils::event
