#pragma once

#include <memory>
#include <type_traits>

#include "driver.hxx"
#include "pointer+fwd.hxx"


namespace p5::lambda::utils::container::pointer {

using Node = parent_::driver::Node;

template <class Interface> struct Type final {
    static_assert(::std::is_class_v<Interface>);

    using Node = this_::Node;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto check(bool = true) const noexcept(true);
    auto & resolve() const noexcept(false);
    auto subscribe(auto &&) const noexcept(false);

    auto * address() const noexcept(true);
    auto & reference() const noexcept(true);

    operator bool () const noexcept(true);
    auto & operator * () const noexcept(true);
    auto * operator -> () const noexcept(true);
    auto operator co_await() const noexcept(true);

    Type() noexcept(true) = default;
    Type(Type &&) noexcept(true) = default;
    Type(Type const &) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;
    Type & operator = (Type const &) noexcept(true) = default;

    explicit Type(::std::weak_ptr<Node const> &&) noexcept(true);

private:
    ::std::weak_ptr<Node const> node_ = {};
};

} // namespace p5::lambda::utils::container::pointer


#include "pointer+inl.hxx"
