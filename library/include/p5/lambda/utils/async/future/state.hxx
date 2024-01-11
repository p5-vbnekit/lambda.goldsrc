#pragma once

#include "state+fwd.hxx"


namespace p5::lambda::utils::async::future::state {

enum class Type { Initial, Ready, Cancelled, Bad };

} // namespace p5::lambda::utils::async::future::state
