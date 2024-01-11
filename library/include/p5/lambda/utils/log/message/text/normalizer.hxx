#pragma once

#include <type_traits>

#include "chunk.hxx"
#include "driver.hxx"
#include "normalizer+fwd.hxx"


namespace p5::lambda::utils::log::message::text::normalizer {
namespace driver {

using Chunk = parent_::parent_::Chunk;
using Interface = parent_::parent_::driver::Interface;

template <> struct Type<this_::Interface *> final: this_::Interface {
    using Chunk = this_::Chunk;
    using Source = this_::Interface *;

    virtual Chunk const * operator () () noexcept(true) override final;

    Type(Type &&) noexcept(true);
    explicit Type(Source = nullptr) noexcept(true);
    virtual ~Type() noexcept(true) override final;

private:
    struct State_ final {
        char skip; Chunk tail, value;
    } state_ = {.skip = 0, .tail = {}, .value = {}};

    Source source_;

    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

template <class T> struct Type final: this_::Interface {
    using Chunk = this_::Chunk;
    using Source = ::std::remove_reference_t<T>;

    static_assert(::std::is_same_v<Source, T>);

    virtual Chunk const * operator () () noexcept(true) override final;

    Type(Type &&) noexcept(true) = default;
    explicit Type(Source &&) noexcept(true);

private:
    struct Adapter_ final: this_::Interface {
        virtual Chunk const * operator () () noexcept(true) override final;

        Adapter_(Adapter_ &&) noexcept(true);
        explicit Adapter_(Source &&) noexcept(true);

    private:
        Source source_;

        Adapter_(Adapter_ const &) = delete;
        Adapter_ & operator = (Adapter_ &&) = delete;
        Adapter_ & operator = (Adapter_ const &) = delete;
    };

    Adapter_ adapter_;
    this_::Type<this_::Interface *> implementation_{&adapter_};

    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace driver
} // namespace namespace p5::lambda::utils::log::message::text::normalizer


#include "normalizer+inl.hxx"
