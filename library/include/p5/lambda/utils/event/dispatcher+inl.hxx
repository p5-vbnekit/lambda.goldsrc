#pragma once

#include <memory>
#include <type_traits>

#include "source.hxx"
#include "handler.hxx"
#include "dispatcher.hxx"
#include "subscription.hxx"


namespace p5::lambda::utils::event::dispatcher {
namespace driver_ {

namespace base = parent_::parent_::source::driver;

using Base = this_::base::Type;
using Event = this_::base::Event;
using Handler = this_::base::Handler;
using Subscription = this_::base::Subscription;

struct Type final: this_::Base {
    using Event = this_::Event;
    using Handler = this_::Handler;
    using Subscription = this_::Subscription;

    void dispatch(Event const &) noexcept(true);
    virtual Subscription subscribe(Handler &&) noexcept(false) override final;
    virtual void unsubscribe(
        Subscription const &
    ) noexcept(false) override final;

    Type() noexcept(false);
    ~Type() noexcept(true);

    Type(Type &&) = default;
    Type & operator = (Type &&) = default;

private:
    struct Private_;
    ::std::unique_ptr<Private_> private_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

template <class Event, class ... T> inline constexpr static
auto pass_pointer_condition_() noexcept(true) {
    if constexpr (1 != sizeof ... (T)) return false;
    else return ::std::is_convertible_v<
        ::std::add_pointer_t<T> ..., Event const *
    >;
}

} // namespace driver_

template <class Event> template <class ... T>
inline auto this_::Type<Event>::dispatch(T && ... event) {
    if constexpr (::std::is_same_v<Event, void>) {
        static_assert(0 == sizeof ... (T));
        driver_.dispatch(nullptr);
    }

    else {
        if constexpr (
            driver_::pass_pointer_condition_<Event, T ...>()
        ) driver_.dispatch(&event ...);
        else {
            Event const event_{::std::forward<T>(event) ...};
            driver_.dispatch(&event_);
        }
    }
}

template <class EventType> inline Type<EventType>::Type() noexcept(false): Type{
    new ::std::decay_t<decltype(driver_)>
} {}

template <class EventType> inline Type<EventType>::Type(
    ::std::decay_t<decltype(driver_)> *driver
) noexcept(false): Base{*driver}, driver_{*driver} {}

} // namespace p5::lambda::utils::event::dispatcher
