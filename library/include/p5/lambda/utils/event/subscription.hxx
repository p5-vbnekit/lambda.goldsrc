#pragma once

#include <memory>

#include "subscription+fwd.hxx"


namespace p5::lambda::utils::event::subscription {

using Entry = this_::storage::Entry;
using Pointer = ::std::weak_ptr<this_::storage::Entry>;

struct Type final {
    using Pointer = this_::Pointer;

    bool state() const noexcept(true);

    bool cancel() noexcept(true);
    bool detach() noexcept(true);
    Type && take() noexcept(true);

    operator bool () const noexcept(true);

    ~Type() noexcept(true);
    explicit Type(Pointer &&) noexcept(false);

    Type() = default;
    Type(Type &&) = default;
    Type(Type const &) = default;

    Type & operator = (Type &&) = default;
    Type & operator = (Type const &) = default;

private:
    friend this_::Entry;
    struct Private_;
    ::std::shared_ptr<Private_> private_ = {};
};

} // namespace p5::lambda::utils::event::subscription


#include "subscription+inl.hxx"
