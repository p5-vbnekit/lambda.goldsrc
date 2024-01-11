#pragma once

#include <coroutine>
#include <type_traits>

#include "state.hxx"
#include "future.hxx"
#include "promise.hxx"
#include "type+fwd.hxx"


namespace p5::lambda::utils::async::task {

template <class T> struct Type final {
    static_assert(! ::std::is_rvalue_reference_v<T>);

    using State = this_::State;
    using Future = this_::Future<T>;
    using Promise = this_::Promise<T>;

    using promise_type = Promise;

    auto unlink() noexcept(true);
    auto linked() const noexcept(true);

    auto state() const noexcept(true);
    auto const & future() const noexcept(true);

    auto stop() const noexcept(true);
    auto start() const noexcept(true);

private:
    Future future_{};
    ::std::coroutine_handle<Promise> coroutine_{};

public:
    Type() noexcept(true) = default;
    Type(::std::decay_t<decltype(coroutine_)> const &) noexcept(false);

    Type(Type &&) noexcept(true);
    Type(Type const &) noexcept(true);

    auto & operator = (Type &&) noexcept(true);
    auto & operator = (Type const &) noexcept(true);

    ~Type() noexcept(true);
};

} // namespace p5::lambda::utils::async::task


#include "type+inl.hxx"
