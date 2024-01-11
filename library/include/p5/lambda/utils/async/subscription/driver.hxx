#pragma once

#include "driver+fwd.hxx"


namespace p5::lambda::utils::async::subscription::driver {

struct Type {
    virtual bool pin() noexcept(true) = 0;
    virtual bool unpin() noexcept(true) = 0;
    virtual bool pinned() const noexcept(true) = 0;

    virtual bool state() const noexcept(true) = 0;
    virtual bool cancel() noexcept(true) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true) = default;

private:
    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::async::subscription::driver
