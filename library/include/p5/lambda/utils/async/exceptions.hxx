#pragma once

#include <stdexcept>

#include "subscription+fwd.hxx"


namespace p5::lambda::utils::async::exceptions {

struct BadState final: ::std::logic_error {
    explicit BadState(char const * = nullptr) noexcept(true);

    BadState(BadState &&) noexcept(true) = default;
    BadState(BadState const &) noexcept(true) = default;

    BadState & operator = (BadState &&) noexcept(true) = default;
    BadState & operator = (BadState const &) noexcept(true) = default;
};

struct Cancelled final: ::std::runtime_error {
    explicit Cancelled(char const * = nullptr) noexcept(true);

    Cancelled(Cancelled &&) noexcept(true) = default;
    Cancelled(Cancelled const &) noexcept(true) = default;

    Cancelled & operator = (Cancelled &&) noexcept(true) = default;
    Cancelled & operator = (Cancelled const &) noexcept(true) = default;
};

} // namespace p5::lambda::utils::async::exceptions
