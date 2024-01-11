#pragma once

#include <cstdint>

#include <optional>
#include <type_traits>

#include "future.hxx"
#include "type+fwd.hxx"
#include "promise+fwd.hxx"


namespace p5::lambda::utils::async::task::promise {

enum class Phase { Initial, Started, Finished };

template <> struct Type<void> final {
    auto get_return_object() noexcept(true);

    constexpr static auto final_suspend() noexcept(true);
    constexpr static auto initial_suspend() noexcept(true);

    constexpr static auto await_transform(auto &&) noexcept(false);

    auto return_void() noexcept(false);
    auto unhandled_exception() noexcept(true);

    Type() noexcept(false);

private:
    friend struct parent_::Type<void>;

    struct State_ final {
        this_::Phase phase{::std::decay_t<decltype(phase)>::Initial};
        ::std::optional<parent_::future::Issuer<void>> future{::std::nullopt};
        bool suspended{false};
        ::std::size_t
#if (defined(__clang_major__) && (15 > __clang_major__))
            volatile
#endif
        references{0};
    } state_ = {};

    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

template <class T> struct Type final {
    static_assert(! ::std::is_void_v<T>);
    static_assert(! ::std::is_rvalue_reference_v<T>);

    auto get_return_object() noexcept(true);

    constexpr static auto final_suspend() noexcept(true);
    constexpr static auto initial_suspend() noexcept(true);

    constexpr static auto await_transform(auto &&) noexcept(false);

    auto return_value(::std::remove_reference_t<T> &) noexcept(false);
    auto return_value(T &&) noexcept(false) requires(
        ! ::std::is_reference_v<T>
    );

    auto unhandled_exception() noexcept(true);

    Type() noexcept(false);

private:
    friend struct parent_::Type<T>;

    struct State_ final {
        this_::Phase phase{::std::decay_t<decltype(phase)>::Initial};
        ::std::optional<parent_::future::Issuer<T>> future{::std::nullopt};
        bool suspended{false};
        ::std::size_t
#if (defined(__clang_major__) && (15 > __clang_major__))
            volatile
#endif
        references{0};
    } state_ = {};

    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::async::task::promise


#include "promise+inl.hxx"
