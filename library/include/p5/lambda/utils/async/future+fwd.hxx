#pragma once

#include "namespace_.hxx"

#include "future/front+fwd.hxx"
#include "future/state+fwd.hxx"
#include "future/value+fwd.hxx"
#include "future/issuer+fwd.hxx"
#include "future/driver+fwd.hxx"
#include "future/handler+fwd.hxx"
#include "future/exceptions+fwd.hxx"
#include "future/subscription+fwd.hxx"


namespace p5::lambda::utils::async {

template <class ... T> using Future = this_::future::Front<T ...>;

} // namespace p5::lambda::utils::async
