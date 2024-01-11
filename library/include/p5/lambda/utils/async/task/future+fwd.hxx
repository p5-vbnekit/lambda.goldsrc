#pragma once

#include "namespace_.hxx"
#include "../future/issuer+fwd.hxx"


namespace p5::lambda::utils::async::task {
namespace future {

namespace parent_ = this_;
namespace this_ = parent_::future;

namespace type {

namespace parent_ = this_;
namespace this_ = parent_::type;

template <class = void> struct Wrapper;

} // namespace type

template <class ... T> using Type = typename this_::type::Wrapper<T ...>::Type;

namespace issuer {

using namespace parent_::parent_::future::issuer;

} // namespace issuer

template <class ... T> using Issuer = this_::issuer::Type<T ...>;

} // namespace future

template <class ... T> using Future = this_::future::Type<T ...>;

} // namespace p5::lambda::utils::async::task
