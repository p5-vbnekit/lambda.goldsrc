#pragma once

#include "value+fwd.hxx"
#include "namespace_.hxx"


namespace p5::lambda::utils::async::future {
namespace handler {

namespace parent_ = this_;
namespace this_ = parent_::handler;

namespace type {

namespace parent_ = this_;
namespace this_ = parent_::type;

template <class = parent_::parent_::value::type::Default> struct Wrapper;

} // namespace type

template <class ... T> using Type = typename this_::type::Wrapper<T ...>::Type;

} // namespace handler

template <class ... T> using Handler = this_::handler::Type<T ...>;

} // namespace p5::lambda::utils::async::future
