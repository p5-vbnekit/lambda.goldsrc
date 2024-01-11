#pragma once

#include <utility>
#include <type_traits>

#include "type.hxx"
#include "driver.hxx"


namespace p5::lambda::utils::async::subscription {

inline auto this_::Type::unlink() noexcept(true) {
    if (! driver_) return false;
    driver_.reset();
    return true;
}

inline auto this_::Type::linked() const noexcept(true) {
    return static_cast<bool>(driver_);
}

inline auto this_::Type::pin() const noexcept(true) {
    return driver_ && driver_->pin();
}

inline auto this_::Type::unpin() const noexcept(true) {
    return driver_ && driver_->unpin();
}

inline auto this_::Type::pinned() const noexcept(true) {
    return driver_ && driver_->pinned();
}

inline auto this_::Type::state() const noexcept(true) {
    return driver_ && driver_->state();
}

inline auto this_::Type::cancel() const noexcept(true) {
    return driver_ && driver_->cancel();
}

inline this_::Type::operator bool () const noexcept(true) { return state(); }

inline this_::Type::Type(
    ::std::decay_t<decltype(driver_)> &&driver
) noexcept(true): driver_{::std::move(driver)} {}

} // namespace p5::lambda::utils::async::subscription
