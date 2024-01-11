#pragma once

#include <type_traits>

#include "chunk+fwd.hxx"
#include "driver+fwd.hxx"


namespace p5::lambda::utils::log::message::text::driver {

using Chunk = parent_::Chunk;

namespace interface {

using Chunk = parent_::Chunk;

struct Type {
    using Chunk = this_::Chunk;

    virtual Chunk const * operator () () noexcept(true) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true);
    Type(Type &&) noexcept(true);
    Type & operator = (Type &&) noexcept(true);

private:
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace interface

template <class T> struct Type final: this_::Interface {
    using Chunk = this_::Chunk;
    using Interface = this_::Interface;

    static_assert(::std::is_same_v<::std::decay_t<T>, T>);

    virtual Chunk const * operator () () noexcept(true) override final;

    constexpr Type(T &&) noexcept(true);

    constexpr Type(Type &&) noexcept(true);
    constexpr Type & operator = (Type &&) noexcept(true);

private:
    bool done_ = false;
    T delegate_;

    Type() = delete;
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::log::message::text::driver


#include "driver+inl.hxx"
