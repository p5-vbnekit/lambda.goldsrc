#pragma once

#include <memory>
#include <type_traits>

#include "type+fwd.hxx"
#include "driver+fwd.hxx"


namespace p5::lambda::utils::async::subscription {

struct Type final {
    using Driver = this_::Driver;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto pin() const noexcept(true);
    auto unpin() const noexcept(true);
    auto pinned() const noexcept(true);

    auto state() const noexcept(true);
    auto cancel() const noexcept(true);

    operator bool () const noexcept(true);

    ~Type() noexcept(true) = default;

    Type() noexcept(true) = default;
    Type(Type &&) noexcept(true) = default;
    Type(Type const &) noexcept(true) = default;

    Type & operator = (Type &&) noexcept(true) = default;
    Type & operator = (Type const &) noexcept(true) = default;

private:
    ::std::shared_ptr<Driver> driver_ = {};

public:
    explicit Type(::std::decay_t<decltype(driver_)> &&driver) noexcept(true);
};

} // namespace p5::lambda::utils::async::subscription


#include "type+inl.hxx"
