#pragma once

#include "state+fwd.hxx"


namespace p5::lambda::utils::async::task::state {

enum class Type { Initial, Started, Finished, Cancelled, Bad };

} // namespace p5::lambda::utils::async::task::state
