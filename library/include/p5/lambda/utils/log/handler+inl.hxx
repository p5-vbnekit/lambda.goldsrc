#pragma once

#include <utility>

#include "type.hxx"


namespace p5::lambda::utils::log::handler {

template <class T> inline auto capture(T &&handler) noexcept(false) {
    return [
        captured_ = ::std::forward<T>(handler)
    ] (auto const &message) { captured_(message); };
}

} // namespace p5::lambda::utils::log::handler
