#pragma once

#include <memory>
#include <utility>
#include <type_traits>

#include "lock+fwd.hxx"


namespace p5::lambda::utils::container::lock {

template <class Interface> struct Type final {
    static_assert(::std::is_class_v<Interface>);

    using Driver = this_::Driver;

    auto && take() noexcept(true);
    auto release() noexcept(true);

    template <class Factory> auto inject(Factory &&) noexcept(false);

    template <class Instance, class Factory> auto inject(
        ::std::in_place_t const &, Factory &&
    ) noexcept(false);

    template <class Instance, class Factory> auto inject(
        ::std::in_place_type_t<Instance> const &, Factory &&
    ) noexcept(false);

    template <class Instance> auto assign(Instance *) noexcept(false);
    template <class Instance> auto assign(Instance &) noexcept(false);

    template <class Instance, class ... T>
    auto emplace(T && ...) noexcept(false);

    explicit Type(::std::unique_ptr<Driver> &&) noexcept(true);

    Type() = default;
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

private:
    ::std::unique_ptr<Driver> driver_ = {};

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::container::lock


#include "lock+inl.hxx"
