#pragma once

#include <ranges>
#include <utility>
#include <variant>
#include <iterator>
#include <exception>
#include <coroutine>
#include <type_traits>

#include "generator+fwd.hxx"

#include "sugar/uncopyable.hxx"


namespace p5::lambda::utils::generator {
namespace function {
namespace machine {

template <class T> struct Type final {
    using Driver = ::std::remove_reference_t<T>;

    static_assert(::std::is_same_v<Driver, T>);
    static_assert(! ::std::is_pointer_v<::std::remove_pointer_t<Driver>>);
    static_assert(requires () {
        ::std::declval<::std::remove_pointer_t<Driver>>()();
    });

    using Pointer = ::std::remove_reference_t<decltype(
        ::std::declval<::std::remove_pointer_t<Driver>>()()
    )>;

    static_assert(::std::is_pointer_v<Pointer>);

    using Value = ::std::remove_reference_t<
        decltype(*::std::declval<Pointer>())
    >;

    constexpr operator bool () noexcept(true);
    constexpr auto & operator * () noexcept(false);
    constexpr auto operator () () noexcept(true);

    constexpr Type() noexcept(true);

    template <class Driver> inline constexpr explicit
    Type(Driver &&driver) noexcept(true) requires(
        ! ::std::is_base_of_v<Type, ::std::decay_t<Driver>>
    ): driver_{::std::forward<Driver>(driver)} {}

    constexpr Type(Type &&) noexcept(true);
    constexpr auto & operator = (Type &&) noexcept(true);

private:
    ::std::variant<::std::monostate, Pointer, ::std::exception_ptr> state_ = {};
    Driver driver_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;

    constexpr auto & reference_() noexcept(false);
};

} // namespace machine

namespace iterator {

template <class Driver> struct Type final {
    constexpr Type & operator ++ () noexcept(true);

#if defined(__GNUC__) && (! defined __clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#endif
    constexpr void operator ++ (int) noexcept(true);
#if defined(__GNUC__) && (! defined __clang__)
#pragma GCC diagnostic pop
#endif

    constexpr auto & operator * () const noexcept(false);

    constexpr bool operator == (
        ::std::default_sentinel_t const &
    ) const noexcept(true);

private:
    parent_::Machine<Driver> *machine_ = nullptr;

public:
    using value_type = typename ::std::decay_t<decltype(*machine_)>::Value;
    using difference_type = ::std::ptrdiff_t;
    using iterator_category = ::std::input_iterator_tag;

    constexpr explicit Type(
        ::std::decay_t<decltype(*machine_)> &
    ) noexcept(true);

    constexpr Type() noexcept(true) = default;
    constexpr Type(Type &&) noexcept(true) = default;
    constexpr Type(Type const &) noexcept(true) = default;
    constexpr Type & operator = (Type &&) noexcept(true) = default;
    constexpr Type & operator = (Type const &) noexcept(true) = default;
};

} // namespace iterator

template <class Driver> struct Type final:
#if ! (defined(__clang_major__) && (16 > __clang_major__))
    ::std::ranges::view_interface<Type<Driver>>,
#endif
    parent_::parent_::sugar::Uncopyable
{
    constexpr auto end() const noexcept(true);
    constexpr auto begin() noexcept(true);

    template <class T> constexpr explicit Type(T &&driver) noexcept(true);

    constexpr Type() noexcept(true) = default;
    constexpr Type(Type &&) noexcept(true) = default;
    constexpr Type & operator = (Type &&) noexcept(true) = default;

private:
    this_::Machine<Driver> machine_;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace function

namespace coroutine {
namespace promise {

template <class T> struct Type final {
    static_assert(::std::is_same_v<::std::remove_reference_t<T>, T>);

    ::std::variant<::std::monostate, T *, ::std::exception_ptr> payload_ = {};

    void await_transform() = delete;
    constexpr auto get_return_object() noexcept(true);
    constexpr static auto initial_suspend() noexcept(true);
    constexpr auto yield_value(T &value) noexcept(true);
    constexpr auto yield_value(T &&value) noexcept(true);
    constexpr static auto return_void() noexcept(true);
    constexpr auto unhandled_exception() noexcept(true);
    constexpr static auto final_suspend() noexcept(true);

    constexpr Type() noexcept(true) = default;

private:
    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace promise

template <class T> using Handle_ = ::std::coroutine_handle<this_::Promise<T>>;

namespace iterator {

template <class T> struct Type final {
    using value_type = ::std::decay_t<decltype(*::std::get<1>(
        ::std::declval<parent_::Promise<T>>().payload_
    ))>;
    using difference_type = ::std::ptrdiff_t;
    using iterator_category = ::std::input_iterator_tag;

    constexpr Type & operator ++ () noexcept(true);

#if defined(__GNUC__) && (! defined __clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#endif
    constexpr void operator ++ (int) noexcept(true);
#if defined(__GNUC__) && (! defined __clang__)
#pragma GCC diagnostic pop
#endif

    constexpr auto & operator * () const noexcept(false);

    constexpr bool operator == (
        ::std::default_sentinel_t const &
    ) const noexcept(true);

    constexpr explicit Type(Handle_<T> const &coroutine) noexcept(true);

private:
    Handle_<T> coroutine_;
};

} // namespace iterator

template <class T> struct Type final:
#if ! (defined(__clang_major__) && (16 > __clang_major__))
    ::std::ranges::view_interface<Type<T>>,
#endif
    parent_::parent_::sugar::Uncopyable
{
    using promise_type = this_::Promise<T>;

    constexpr auto end() const noexcept(true);
    constexpr auto begin() const noexcept(true);

    constexpr explicit Type(Handle_<T> const &coroutine) noexcept(true);

    ~Type() noexcept(true);
    constexpr Type() noexcept(true) = default;
    constexpr Type(Type &&other) noexcept(true);
    constexpr auto & operator = (Type &&other) noexcept(true);

private:
    Handle_<T> coroutine_;
};

} // namespace coroutine
} // namespace p5::lambda::utils::generator


#include "generator+inl.hxx"
