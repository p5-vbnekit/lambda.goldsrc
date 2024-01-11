#pragma once

#include "subscription.hxx"


namespace p5::lambda::utils::event::subscription {

inline this_::Type::operator bool () const noexcept(true) { return state(); }

} // namespace p5::lambda::utils::event::subscription
