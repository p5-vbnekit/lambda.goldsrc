#pragma once

#include "event.hxx"
#include "handler.hxx"
#include "driver+fwd.hxx"
#include "subscription.hxx"


namespace p5::lambda::utils::async::channel::driver {

using Event = parent_::Event<void const *>;
using Handler = parent_::Handler<Event>;
using Subscription = parent_::Subscription;

struct Type {
    using Event = this_::Event;
    using Handler = this_::Handler;
    using Subscription = this_::Subscription;

    virtual bool dispatch(void const *) noexcept(true) = 0;
    virtual Subscription subscribe(Handler &&) noexcept(false) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true) = default;

private:
    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::async::channel::driver
