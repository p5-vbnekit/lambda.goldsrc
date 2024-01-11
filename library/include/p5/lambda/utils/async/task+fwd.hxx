#pragma once

#include "namespace_.hxx"

#include "task/type+fwd.hxx"
#include "task/state+fwd.hxx"
#include "task/future+fwd.hxx"
#include "task/promise+fwd.hxx"
#include "task/exceptions+fwd.hxx"


namespace p5::lambda::utils::async {

template <class ... T> using Task = this_::task::Type<T ...>;

} // namespace p5::lambda::utils::async
