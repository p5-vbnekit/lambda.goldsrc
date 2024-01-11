#pragma once

#include <memory>

#include "lock+fwd.hxx"
#include "type+fwd.hxx"
#include "driver+fwd.hxx"
#include "pointer+fwd.hxx"


namespace p5::lambda::utils::container {

struct Type final {
    template <class T> using Lock = this_::Lock<T>;
    template <class T> using Pointer = this_::Pointer<T>;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    template <class Interface> auto get() const noexcept(false);
    template <class Interface> auto lock() noexcept(false);

    template <class Interface = void> auto assign(auto &&) noexcept(false);
    template <class Interface = void, class Instance = Interface>
    auto emplace(auto && ...) noexcept(false);

    Type() noexcept(false);
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

    ~Type() noexcept(true);

private:
    ::std::unique_ptr<this_::Driver> driver_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::container


#include "type+inl.hxx"
