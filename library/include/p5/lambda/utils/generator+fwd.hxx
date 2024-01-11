#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils {
namespace generator {

namespace parent_ = this_;
namespace this_ = parent_::generator;

namespace function {

namespace parent_ = this_;
namespace this_ = parent_::function;

namespace machine {

namespace parent_ = this_;
namespace this_ = parent_::machine;

template <class> struct Type;

} // namespace machine

template <class Driver> using Machine = machine::Type<Driver>;

namespace iterator {

namespace parent_ = this_;
namespace this_ = parent_::iterator;

template <class> struct Type;

} // namespace iterator

template <class Driver> using Iterator = iterator::Type<Driver>;

template <class = void> struct Type;

template <class Driver> constexpr auto make(Driver &&) noexcept(true);

} // namespace function

template <class T> using Function = function::Type<T>;

namespace coroutine {

namespace parent_ = this_;
namespace this_ = parent_::coroutine;

namespace promise {

namespace parent_ = this_;
namespace this_ = parent_::promise;

template <class> struct Type;

} // namespace promise

template <class T> using Promise = promise::Type<T>;

namespace iterator {

namespace parent_ = this_;
namespace this_ = parent_::iterator;

template <class> struct Type;

} // namespace iterator

template <class T> using Iterator = iterator::Type<T>;

template <class> struct Type;

template <class T> constexpr auto from_range(T &&);

} // namespace coroutine

template <class T> using Coroutine = coroutine::Type<T>;

} // namespace generator
} // namespace p5::lambda::utils
