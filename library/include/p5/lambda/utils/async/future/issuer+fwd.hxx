#pragma once

#include "value+fwd.hxx"
#include "namespace_.hxx"


namespace p5::lambda::utils::async::future {
namespace issuer {

namespace parent_ = this_;
namespace this_ = parent_::issuer;

template <class = parent_::value::type::Default> struct Type;

} // namespace issuer

template <class ... T> using Issuer = this_::issuer::Type<T ...>;

} // namespace p5::lambda::utils::async::future
