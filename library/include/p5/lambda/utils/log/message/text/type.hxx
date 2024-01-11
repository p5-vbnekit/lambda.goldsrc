#pragma once

#include "type+fwd.hxx"
#include "chunk+fwd.hxx"
#include "driver+fwd.hxx"


namespace p5::lambda::utils::log::message::text {

struct Type final {
    using Chunk = this_::Chunk;
    using Driver = this_::driver::Interface;

    auto operator * () noexcept(true);
    auto * operator () () noexcept(true);

    constexpr explicit Type(Driver * = nullptr) noexcept(true);
    constexpr explicit Type(Driver &) noexcept(true);

    constexpr Type(Type &&) noexcept(true);
    constexpr auto & operator = (Type &&) noexcept(true);

private:
    Driver *driver_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::log::message::text


#include "type+inl.hxx"
