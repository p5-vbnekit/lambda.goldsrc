#pragma once

#include <memory>
#include <exception>

#include "state.hxx"
#include "handler.hxx"
#include "driver+fwd.hxx"
#include "subscription.hxx"


namespace p5::lambda::utils::async::future::driver {

using State = parent_::State;
using Handler = parent_::Handler<void>;
using Pointer = ::std::unique_ptr<void, void(*)(void const *)>;
using Exception = ::std::exception_ptr;
using Subscription = parent_::Subscription;

struct Type {
    using State = this_::State;
    using Handler = this_::Handler;
    using Pointer = this_::Pointer;
    using Exception = this_::Exception;
    using Subscription = this_::Subscription;

    virtual void set(Pointer &&) noexcept(false) = 0;

    virtual void * get() const noexcept(true) = 0;
    virtual void * get_or_throw() const noexcept(false) = 0;

    virtual State state() const noexcept(true) = 0;

    virtual Exception exception() const noexcept(true) = 0;
    virtual bool exception(Exception const &) noexcept(true) = 0;

    virtual bool cancel() noexcept(true) = 0;
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

} // namespace p5::lambda::utils::async::future::driver
