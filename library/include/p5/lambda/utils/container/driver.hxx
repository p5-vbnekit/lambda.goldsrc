#pragma once

#include <bitset>
#include <memory>
#include <typeindex>
#include <functional>

#include "driver+fwd.hxx"
#include "../async/subscription.hxx"


namespace p5::lambda::utils::container::driver {

struct Type {
    using Node = this_::Node;

    virtual ::std::shared_ptr<Node> const & node(
        ::std::type_index const &
    ) noexcept(false) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true);

private:
    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

namespace node {
namespace payload {

namespace parent_ = this_;
namespace this_ = parent_::payload;

using Pointer = ::std::unique_ptr<void, void(*)(void const *)>;
using Qualifiers = ::std::bitset<2>;

struct Type final {
    Pointer pointer = {nullptr, nullptr};
    Qualifiers qualifiers = 0;

    bool operator != (Type const &) noexcept(true);
    bool operator == (Type const &) noexcept(true);
};

} // namespace payload

using Payload = this_::payload::Type;
using Subscription = parent_::parent_::parent_::async::Subscription;

struct Type {
    using Payload = this_::Payload;
    using Subscription = this_::Subscription;

    virtual bool lock(bool) noexcept(true) = 0;

    virtual void payload(Payload &&) noexcept(true) = 0;
    virtual Payload const & payload() const noexcept(true) = 0;

    virtual Subscription subscribe(
        ::std::function<void(void)> &&
    ) const noexcept(false) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true);

private:
    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace node
} // namespace p5::lambda::utils::container::driver


#include "driver+inl.hxx"
