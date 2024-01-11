#pragma once

#include <memory>
#include <type_traits>

#include "driver.hxx"
#include "lock+fwd.hxx"


namespace p5::lambda::utils::container::lock {

using Node = parent_::driver::Node;

template <class Interface> struct Type final {
    static_assert(::std::is_class_v<Interface>);

    using Node = this_::Node;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto pin() noexcept(true);
    auto unpin() noexcept(true);
    auto pinned() const noexcept(true);

    auto release() noexcept(true);

    auto assign(auto *) noexcept(false);
    auto assign(auto &) noexcept(false);

    template <class Instance = Interface>
    auto emplace(auto && ...) noexcept(false);

    Type() noexcept(true);
    Type(Type &&) noexcept(true);
    auto & operator = (Type &&) noexcept(true);
    ~Type() noexcept(true);

    explicit Type(::std::shared_ptr<Node> const &) noexcept(false);

private:
    ::std::weak_ptr<Node> node_ = {};
    bool pinned_ = false;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::container::lock


#include "lock+inl.hxx"
