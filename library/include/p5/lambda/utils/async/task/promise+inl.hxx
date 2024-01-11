#pragma once

#include <utility>
#include <variant>
#include <optional>
#include <exception>
#include <coroutine>
#include <stdexcept>
#include <type_traits>

#include "promise.hxx"
#include "exceptions.hxx"


namespace p5::lambda::utils::async::task::promise {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

namespace exceptions = parent_::parent_::exceptions;

template <class Policy> inline constexpr static
auto final_suspend() noexcept(true) {
    class Result_ final {
        bool suspended_{false};

        Result_(Result_ const &) = delete;
        Result_ & operator = (Result_ &&) = delete;
        Result_ & operator = (Result_ const &) = delete;

    public:
        inline constexpr static auto await_ready() noexcept(true) {
            return false;
        }

        inline auto await_suspend(
            typename Policy::Coroutine const &coroutine
        ) noexcept(true) {
            if (::std::exchange(suspended_, true)) return false;
            if (! coroutine) return false;
            if (! coroutine.done()) return false;
            auto &state_ = Policy::state(coroutine);
            if (! (0 < state_.references)) return false;
            state_.phase = ::std::decay_t<decltype(state_.phase)>::Finished;
            state_.suspended = true;
            return true;
        }

        inline constexpr static auto await_resume() noexcept(true) {}

        Result_() noexcept(true) = default;
        Result_(Result_ &&) noexcept(true) = default;
    };

    return Result_{};
}

template <class Policy> inline constexpr static
auto initial_suspend() noexcept(true) {
    using Coroutine_ = typename Policy::Coroutine;

    class Result_ final {
        ::std::optional<Coroutine_> coroutine_{::std::nullopt};

        Result_(Result_ const &) = delete;
        Result_ & operator = (Result_ &&) = delete;
        Result_ & operator = (Result_ const &) = delete;

    public:
        inline constexpr static auto await_ready() noexcept(true) {
            return false;
        }

        inline auto await_suspend(
            Coroutine_ const &coroutine
        ) noexcept(true) {
            if (coroutine_) return;
            coroutine_.emplace(nullptr);
            if (! coroutine) return;
            if (coroutine.done()) return;
            auto &state_ = Policy::state(coroutine);
            if (state_.suspended) return;
            if (::std::decay_t<
                decltype(state_.phase)
            >::Initial != state_.phase) return;
            if (! state_.future) return;
            auto const future_ = state_.future->state();
            if (::std::decay_t<decltype(future_)>::Initial != future_) return;
            state_.suspended = true;
            coroutine_.emplace(coroutine);
        }

        inline auto await_resume() noexcept(false) {
            if ([this] {
                if (! this->coroutine_) return true;
                auto const coroutine_ = ::std::exchange(
                    *(this->coroutine_), nullptr
                );
                if (! coroutine_) return true;
                if (coroutine_.done()) return true;
                auto &state_ = Policy::state(coroutine_);
                if (! state_.suspended) return true;
                using Phase_ = ::std::decay_t<
                    decltype(state_.phase)
                >;
                if (Phase_::Initial != state_.phase) return true;
                state_.suspended = false;
                if (! state_.future) return true;
                auto const future_ = state_.future->state();
                using Future_ = ::std::decay_t<decltype(future_)>;
                switch (future_) {
                default: return true;
                case Future_::Initial: break;
                case Future_::Cancelled:
                    throw this_::exceptions::Cancelled{};
                }
                state_.phase = Phase_::Started;
                return false;
            } ()) throw this_::exceptions::BadState{"bad task state"};
        }

        Result_() noexcept(true) = default;
        Result_(Result_ &&) noexcept(true) = default;
    };

    return Result_{};
}

namespace awaitable_traits_ {

namespace parent_ = this_;
namespace this_ = parent_::awaitable_traits_;

template <class T> struct TypeHolder_ final { using Type = T; };

template <class T> inline static
auto make_type_holder_(T &&awaitable) noexcept(false) {
    if constexpr (
        requires () { ::std::forward<T>(awaitable).operator co_await(); }
    ) return this_::TypeHolder_<decltype(
        ::std::forward<T>(awaitable).operator co_await().await_resume()
    )>{};
    else return this_::TypeHolder_<decltype(
        ::std::forward<T>(awaitable).await_resume()
    )>{};
}

template <class T> using Result = typename ::std::decay_t<decltype(
    this_::make_type_holder_(::std::declval<T>())
)>::Type;

} // namespace awaitable_traits_

template <class Policy> inline constexpr static
auto await_transform(auto &&awaitable) noexcept(false) {
    using Awaitable_ = decltype(awaitable);
    using Coroutine_ = typename Policy::Coroutine;

    using ChildValue_ = this_::awaitable_traits_::Result<Awaitable_>;

    using ChildResult_ = ::std::variant<
        ::std::monostate, ::std::exception_ptr,
        ::std::remove_reference_t<ChildValue_> *
    >;

    struct ChildPromise_;

    using ChildCoroutine_ = ::std::coroutine_handle<ChildPromise_>;

    struct ChildPromise_ final {
        struct Context_ final {
            Coroutine_ parent{};
            ChildResult_ result{::std::monostate{}};
        } context_{};

        inline auto get_return_object() noexcept(true) {
            return ChildCoroutine_::from_promise(*this);
        }

        inline constexpr static auto final_suspend() noexcept(true) {
            struct Result_ final {
                inline constexpr static auto await_ready() noexcept(true) {
                    return false;
                }

                inline static auto await_suspend(
                    ChildCoroutine_ const &coroutine
                ) noexcept(true) { return coroutine.promise().context_.parent; }

                inline constexpr static auto await_resume() noexcept(true) {}
            };

            return Result_{};
        }

        inline constexpr static auto initial_suspend() noexcept(true) {
            return ::std::suspend_always{};
        }

        inline auto return_value(::std::decay_t<decltype(
            ::std::get<2>(::std::declval<ChildResult_>())
        )> pointer) noexcept(false) {
            auto &result_ = context_.result;
            if (0 != result_.index()) throw this_::exceptions::BadState{
                "bad task state"
            };
            else result_.template emplace<2>(pointer);
        }

        inline auto unhandled_exception() noexcept(true) {
            context_.result.template emplace<1>(::std::current_exception());
        }
    };

    class Result_ final {
        ChildCoroutine_ child_;

        Result_() = delete;
        Result_(Result_ const &) = delete;
        Result_ & operator = (Result_ &&) = delete;
        Result_ & operator = (Result_ const &) = delete;

    public:
        inline constexpr static auto await_ready() noexcept(true) {
            return false;
        }

        inline auto await_suspend(Coroutine_ const &coroutine) noexcept(false) {
            if (
                (! coroutine) || coroutine.done()
            ) throw ::std::invalid_argument{"bad coroutine"};
            if ([this, &coroutine] {
                auto &coroutine_ = child_.promise().context_.parent;
                if (coroutine_) return true;
                auto &state_ = Policy::state(coroutine_ = coroutine);
                if (::std::decay_t<
                    decltype(state_.phase)
                >::Started != state_.phase) return true;
                if (! state_.future) return true;
                if (state_.suspended) return true;
                auto const future_ = state_.future->state();
                using Future_ = ::std::decay_t<decltype(future_)>;
                switch (future_) {
                default: return true;
                case Future_::Initial: break;
                case Future_::Cancelled:
                    throw this_::exceptions::Cancelled{};
                }
                if (! (0 < state_.references)) return true;
                state_.suspended = true;
                return false;
            } ()) throw this_::exceptions::BadState{"bad task state"};
            return child_;
        }

        inline decltype(auto) await_resume() noexcept(false) {
            auto &context_ = child_.promise().context_;
            if ([&context_] {
                auto &state_ = Policy::state(context_.parent);
                if (! ::std::exchange(state_.suspended, false)) return true;
                if (::std::decay_t<
                    decltype(state_.phase)
                >::Started != state_.phase) return true;
                if (! state_.future) return true;
                auto const future_ = state_.future->state();
                using Future_ = ::std::decay_t<decltype(future_)>;
                switch (future_) {
                default: return true;
                case Future_::Initial: break;
                case Future_::Cancelled:
                    throw this_::exceptions::Cancelled{};
                }
                return ! (0 < state_.references);
            } ()) throw this_::exceptions::BadState{"bad task state"};
            auto const &result_ = context_.result;
            switch(context_.result.index()) {
            default: throw this_::exceptions::BadState{"bad task state"};
            case 2: break;
            case 1:
                if (
                    auto const exception_ = ::std::get<1>(result_)
                ) ::std::rethrow_exception(exception_);
                throw this_::exceptions::BadState{"bad task state"};
            }
            if constexpr (
                ! ::std::is_void_v<ChildValue_>
            ) return ::std::forward<ChildValue_>(*::std::get<2>(result_));
        }

        Result_(Result_ &&) noexcept(true) = default;

        inline explicit Result_(ChildCoroutine_ &&child) noexcept(true):
            child_{::std::move(child)}
        {}

        inline ~Result_() noexcept(true) { child_.destroy(); }
    };

    struct ChildTask_ final {
        using promise_type = ChildPromise_;

        static_assert(::std::is_same_v<ChildPromise_, promise_type>);

        ChildCoroutine_ coroutine;

        inline ChildTask_(
            ChildCoroutine_ const &coroutine
        ) noexcept(true): coroutine{coroutine} {}
    };

    return Result_{[] (Awaitable_ awaitable) -> ChildTask_ {
        if constexpr (::std::is_void_v<ChildValue_>) {
            co_await ::std::forward<Awaitable_>(awaitable);
            co_return nullptr;
        }
        else {
            auto &&value_ = co_await ::std::forward<Awaitable_>(awaitable);
            co_return &value_;
        }
    } (::std::forward<Awaitable_>(awaitable)).coroutine};
}

inline static auto return_value_or_void(
    auto &state, auto && ... value
) noexcept(false) {
    if ([&state] {
        if (::std::decay_t<
            decltype(state.phase)
        >::Started != state.phase) return true;
        if (state.suspended) return true;
        return ! state.future;
    } ()) throw this_::exceptions::BadState{"bad task state"};
    try { state.future->set(::std::forward<decltype(value)>(value) ...); }
    catch (this_::exceptions::Cancelled const &) { throw; }
    catch (...) {
        ::std::throw_with_nested(this_::exceptions::BadState{
            "bad task state: bad future"
        });
        throw this_::exceptions::BadState{
            "bad task state: bad future"
        };
    }
}

inline auto unhandled_exception(
    auto &state, auto const &exception
) noexcept(true) {
    if (state.future && (! (exception && (
        state.future->exception() || state.future->exception(exception)
    )))) state.future = ::std::nullopt;
}

} // namespace private_

inline auto this_::Type<void>::get_return_object() noexcept(true) {
    return ::std::coroutine_handle<
        ::std::decay_t<decltype(*this)>
    >::from_promise(*this);
}

inline constexpr auto this_::Type<void>::final_suspend() noexcept(true) {
    struct Policy_ final {
        using Coroutine = ::std::decay_t<
            decltype(::std::declval<Type>().get_return_object())
        >;
        inline static auto & state(
            Coroutine const &coroutine
        ) noexcept(true) { return coroutine.promise().state_; }
    };
    return this_::private_::final_suspend<Policy_>();
}

inline constexpr auto this_::Type<void>::initial_suspend() noexcept(true) {
    struct Policy_ final {
        using Coroutine = ::std::decay_t<
            decltype(::std::declval<Type>().get_return_object())
        >;
        inline static auto & state(
            Coroutine const &coroutine
        ) noexcept(true) { return coroutine.promise().state_; }
    };
    return this_::private_::initial_suspend<Policy_>();
}

inline constexpr auto this_::Type<void>::await_transform(
    auto &&awaitable
) noexcept(false) {
    struct Policy_ final {
        using Coroutine = ::std::decay_t<
            decltype(::std::declval<Type>().get_return_object())
        >;
        inline static auto & state(
            Coroutine const &coroutine
        ) noexcept(true) { return coroutine.promise().state_; }
    };
    return this_::private_::await_transform<Policy_>(
        ::std::forward<decltype(awaitable)>(awaitable)
    );
}

inline auto this_::Type<void>::return_void() noexcept(false) {
    return this_::private_::return_value_or_void(state_);
}

inline auto this_::Type<void>::unhandled_exception() noexcept(true) {
    return this_::private_::unhandled_exception(
        state_, ::std::current_exception()
    );
}

inline this_::Type<void>::Type() noexcept(false) {
    state_.future.emplace();
    state_.future->subscribe([this] {
        if (! state_.suspended) return;
        using Phase_ = ::std::decay_t<decltype(state_.phase)>;
        switch (state_.phase) {
        default: return;
        case Phase_::Initial: break;
        case Phase_::Started: break;
        }
        get_return_object().resume();
    }).pin();
}

template <class T> inline
auto this_::Type<T>::get_return_object() noexcept(true) {
    return ::std::coroutine_handle<
        ::std::decay_t<decltype(*this)>
    >::from_promise(*this);
}

template <class T> inline constexpr
auto this_::Type<T>::final_suspend() noexcept(true) {
    struct Policy_ final {
        using Coroutine = ::std::decay_t<
            decltype(::std::declval<Type>().get_return_object())
        >;
        inline static auto & state(
            Coroutine const &coroutine
        ) noexcept(true) { return coroutine.promise().state_; }
    };
    return this_::private_::final_suspend<Policy_>();
}

template <class T> inline constexpr
auto this_::Type<T>::initial_suspend() noexcept(true) {
    struct Policy_ final {
        using Coroutine = ::std::decay_t<
            decltype(::std::declval<Type>().get_return_object())
        >;
        inline static auto & state(
            Coroutine const &coroutine
        ) noexcept(true) { return coroutine.promise().state_; }
    };
    return this_::private_::initial_suspend<Policy_>();
}

template <class T> inline constexpr
auto this_::Type<T>::await_transform(auto &&awaitable) noexcept(false) {
    struct Policy_ final {
        using Coroutine = ::std::decay_t<
            decltype(::std::declval<Type>().get_return_object())
        >;
        inline static auto & state(
            Coroutine const &coroutine
        ) noexcept(true) { return coroutine.promise().state_; }
    };
    return this_::private_::await_transform<Policy_>(
        ::std::forward<decltype(awaitable)>(awaitable)
    );
}

template <class T> inline
auto this_::Type<T>::return_value(
    ::std::remove_reference_t<T> &value
) noexcept(false) {
    return this_::private_::return_value_or_void(state_, value);
}

template <class T> inline
auto this_::Type<T>::return_value(T &&value) noexcept(false) requires(
    ! ::std::is_reference_v<T>
) {
    return this_::private_::return_value_or_void(state_, ::std::move(value));
}

template <class T> inline
auto this_::Type<T>::unhandled_exception() noexcept(true) {
    return this_::private_::unhandled_exception(
        state_, ::std::current_exception()
    );
}

template <class T> inline this_::Type<T>::Type() noexcept(false) {
    state_.future.emplace();
    state_.future->subscribe([this] {
        if (! state_.suspended) return;
        using Phase_ = ::std::decay_t<decltype(state_.phase)>;
        switch (state_.phase) {
        default: return;
        case Phase_::Initial: break;
        case Phase_::Started: break;
        }
        get_return_object().resume();
    }).pin();
}

} // namespace p5::lambda::utils::async::task::promise
