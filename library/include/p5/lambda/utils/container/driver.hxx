#pragma once

#include <bitset>
#include <memory>
#include <typeindex>
#include <exception>
#include <functional>

#include "lock.hxx"
#include "driver+fwd.hxx"
#include "pointer+fwd.hxx"
#include "../event/subscription.hxx"


namespace p5::lambda::utils::container::driver {

using Lock = parent_::lock::Driver;
using Pointer = parent_::pointer::Driver;
using Qualifiers = ::std::bitset<2>;
using Subscription = parent_::parent_::event::Subscription;

struct Type {
    using Lock = this_::Lock;
    using Pointer = this_::Pointer;
    using Qualifiers = this_::Qualifiers;
    using Subscription = this_::Subscription;

    virtual ::std::shared_ptr<Pointer> get(
        ::std::type_index const &, Qualifiers const &
    ) const noexcept(false) = 0;

    virtual ::std::unique_ptr<Lock> lock(
        ::std::type_index &&, Qualifiers &&
    ) noexcept(false) = 0;

    virtual void collect() noexcept(true) = 0;

    virtual Subscription on_error(::std::function<
        void(::std::type_index const &, ::std::exception_ptr const &)
    > &&) const noexcept(false) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true) = default;
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

private:
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::container::driver
