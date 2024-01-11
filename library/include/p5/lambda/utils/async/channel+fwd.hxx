#pragma once

#include "namespace_.hxx"

#include "channel/front+fwd.hxx"
#include "channel/event+fwd.hxx"
#include "channel/issuer+fwd.hxx"
#include "channel/driver+fwd.hxx"
#include "channel/handler+fwd.hxx"
#include "channel/exceptions+fwd.hxx"
#include "channel/subscription+fwd.hxx"


namespace p5::lambda::utils::async {

template <class ... T> using Channel = this_::channel::Front<T ...>;

} // namespace p5::lambda::utils::async
