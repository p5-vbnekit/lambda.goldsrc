#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::log {
namespace raii {

namespace parent_ = this_;
namespace this_ = parent_::raii;

namespace item {

namespace parent_ = this_;
namespace this_ = parent_::item;

template <class> struct Type;

} // namespace item

template <class T> using Item = item::Type<T>;

namespace context {

namespace parent_ = this_;
namespace this_ = parent_::context;

struct Type;

void dispatch(Type &) noexcept(true);

} // namespace context

using Context = context::Type;

struct Type;

} // namespace raii

using RAII = raii::Type;

} // namespace p5::lambda::utils::log
