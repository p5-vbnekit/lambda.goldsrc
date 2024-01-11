#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::async::channel {
namespace event {

namespace parent_ = this_;
namespace this_ = parent_::event;

namespace type {

namespace parent_ = this_;
namespace this_ = parent_::type;

using Default = void;

template <class = this_::Default> struct Wrapper;

} // namespace type

template <class ... T> using Type = typename this_::type::Wrapper<T ...>::Type;

} // namespace event

template <class ... T> using Event = this_::event::Type<T ...>;

} // namespace p5::lambda::utils::async::channel
