#pragma once

#include <type_traits>

#include "type+fwd.hxx"

#include "raii.hxx"
#include "handler.hxx"
#include "message.hxx"
#include "interface.hxx"


namespace p5::lambda::utils::log {

struct Type final: Interface {
    using RAII = this_::RAII;
    using Handler = this_::Handler;
    using Message = this_::Message;
    using Interface = this_::Interface;

    Handler handler = {};

    template <class T> auto operator >> (T &&) noexcept(false) requires(
        ::std::is_convertible_v<T, Handler>
    );

    template <class T> explicit Type(T &&) noexcept(false) requires(
        ! ::std::is_base_of_v<Interface, ::std::decay_t<T>>
    );

    Type() = default;
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

private:
    virtual void write_(Message &&) const noexcept(true) override final;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::log


#include "type+inl.hxx"
