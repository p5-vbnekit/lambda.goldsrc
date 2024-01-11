#define BOOST_TEST_MODULE utils.async
#include <boost/test/unit_test.hpp>

#include <utility>
#include <optional>
#include <coroutine>
#include <exception>
#include <stdexcept>
#include <type_traits>

#include <p5/lambda/utils/async.hxx>


namespace target_ = ::p5::lambda::utils::async;

BOOST_AUTO_TEST_SUITE(async_suite)

BOOST_AUTO_TEST_CASE(task_case) {
    using TaskState_ = target_::task::State;

    if constexpr (true) {
        auto const task_ = [] () -> target_::Task<void> {
            co_return co_await ::std::suspend_never{};
        } ();

        BOOST_CHECK(TaskState_::Initial == task_.state());
        BOOST_CHECK(task_.start());
        BOOST_CHECK(TaskState_::Finished == task_.state());
        BOOST_CHECK(! task_.start());
        BOOST_CHECK(TaskState_::Finished == task_.state());
        BOOST_CHECK(! task_.stop());
        BOOST_CHECK(TaskState_::Finished == task_.state());
    }

    if constexpr (true) {
        auto const task_ = [] () -> target_::Task<struct Payload_ *> {
            co_await ::std::suspend_always{};
            co_return nullptr;
        } ();

        BOOST_CHECK(TaskState_::Initial == task_.state());
        BOOST_CHECK(task_.start());
        BOOST_CHECK(TaskState_::Started == task_.state());
        BOOST_CHECK(! task_.start());
        BOOST_CHECK(TaskState_::Started == task_.state());
        BOOST_CHECK(task_.stop());
        BOOST_CHECK(TaskState_::Cancelled == task_.state());
    }

    if constexpr (true) {
        auto future_ = target_::future::Issuer<int>{};
        auto const task_ = [] (auto future) -> target_::Task<int> {
            co_await ::std::suspend_never{};
            co_return 37 + co_await future;
        } (future_.front());

        BOOST_CHECK(TaskState_::Initial == task_.state());
        BOOST_CHECK(task_.start());
        BOOST_CHECK(TaskState_::Started == task_.state());
        BOOST_CHECK(! task_.start());
        BOOST_CHECK(TaskState_::Started == task_.state());
        BOOST_CHECK_NO_THROW([&future_] {future_.set(42); } ());
        BOOST_CHECK(TaskState_::Finished == task_.state());
        BOOST_CHECK(! task_.start());
        BOOST_CHECK(! task_.stop());

        auto const &task_future_ = task_.future();
        BOOST_CHECK(::std::decay_t<
            decltype(task_future_.state())
        >::Ready == task_future_.state());
        auto const * const pointer_ = task_future_.get();
        BOOST_CHECK(static_cast<bool>(pointer_));
        if (pointer_) BOOST_CHECK_EQUAL(37 + 42, *pointer_);
    }
}

BOOST_AUTO_TEST_CASE(future_case) {
    target_::future::Issuer<int> issuer_;
    static_assert(::std::is_same_v<
        target_::Future<int> const,
        ::std::remove_reference_t<decltype(issuer_.front())>
    >);
    static_assert(::std::is_same_v<
        target_::future::Front<int> const,
        ::std::remove_reference_t<decltype(issuer_.front())>
    >);

    auto front_ = issuer_.front();
    static_assert(::std::is_same_v<
        target_::Future<int>,
        ::std::remove_reference_t<decltype(front_)>
    >);
    static_assert(::std::is_same_v<
        target_::future::Front<int>,
        ::std::remove_reference_t<decltype(front_)>
    >);

    auto state_ = front_.state();
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Initial == state_);
    BOOST_CHECK(issuer_.state() == state_);

    auto subscription_ = front_.subscribe([&state_, &front_] {
        state_ = front_.state();
    });

    BOOST_CHECK(static_cast<bool>(subscription_));
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Initial == state_);
    BOOST_CHECK(issuer_.state() == state_);

    auto *pointer_ = front_.get();
    static_assert(::std::is_same_v<int *, ::std::decay_t<decltype(pointer_)>>);
    BOOST_CHECK_EQUAL(nullptr, pointer_);

    issuer_.set(42);
    BOOST_CHECK(! static_cast<bool>(subscription_));
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Ready == state_);
    BOOST_CHECK(issuer_.state() == state_);

    pointer_ = front_.get(::std::rethrow_exception);
    BOOST_CHECK(static_cast<bool>(pointer_));
    BOOST_CHECK_EQUAL(issuer_.get(), pointer_);
    if (pointer_) BOOST_CHECK_EQUAL(42, *pointer_);

    issuer_ = {};
    BOOST_CHECK(static_cast<bool>(front_));
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Bad == front_.state());
    BOOST_CHECK(! static_cast<bool>(issuer_));
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Initial == issuer_.state());

    front_ = issuer_.front();
    BOOST_CHECK(! static_cast<bool>(front_));
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Initial == front_.state());

    auto task_ = [front_] {
        using Value_ = typename ::std::decay_t<decltype(front_)>::Value;

        struct Result_ final {
            struct promise_type final {
                Value_ *pointer_ = nullptr;

                inline auto get_return_object() noexcept(true) {
                    return ::std::coroutine_handle<
                        promise_type
                    >::from_promise(*this);
                }

                inline constexpr static auto initial_suspend() noexcept(true) {
                    return ::std::suspend_never{};
                };

                inline constexpr static auto final_suspend() noexcept(true) {
                    return ::std::suspend_always{};
                };

                inline auto return_value(Value_ &value) noexcept(true) {
                    pointer_ = &value;
                }

                inline static auto unhandled_exception() noexcept(true) {
                    ::std::terminate();
                }
            };

            inline auto done() const noexcept(true) { return handle_.done(); }
            inline auto * pointer() const noexcept(true) {
                return handle_.promise().pointer_;
            }

            Result_(Result_ &&) noexcept(true) = default;

            inline ~Result_() noexcept(true) { handle_.destroy(); }

            inline Result_(
                ::std::coroutine_handle<promise_type> const &handle
            ) noexcept(true): handle_{handle} {}

        private:
            ::std::coroutine_handle<promise_type> const handle_;

            Result_() = delete;
            Result_(Result_ const &) = delete;
            Result_ & operator = (Result_ &&) = delete;
            Result_ & operator = (Result_ const &) = delete;
        };

        return [] (::std::decay_t<decltype(front_)> future) -> Result_ {
            co_await target_::Future<void>{future};
            co_return co_await future;
        } (::std::move(front_));
    } ();

    BOOST_CHECK(! task_.done());
    BOOST_CHECK(! task_.pointer());

    issuer_.set(37);
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Ready == front_.state());
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Ready == issuer_.state());
    pointer_ = task_.pointer();
    BOOST_CHECK(static_cast<bool>(pointer_));
    BOOST_CHECK_EQUAL(front_.get(nullptr), pointer_);
    BOOST_CHECK_EQUAL(issuer_.get(::std::rethrow_exception), pointer_);
    if (pointer_) BOOST_CHECK_EQUAL(37, *pointer_);

    auto const exception_ = ::std::make_exception_ptr(
        ::std::runtime_error{"test"}
    );

    issuer_ = {};
    BOOST_CHECK(static_cast<bool>(front_));
    BOOST_CHECK(! static_cast<bool>(issuer_));
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Bad == front_.state());
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Initial == issuer_.state());

    BOOST_CHECK(issuer_.exception(exception_));
    BOOST_CHECK(static_cast<bool>(issuer_));
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Ready == issuer_.state());
    BOOST_CHECK(exception_ == issuer_.exception());

    BOOST_CHECK(! issuer_.cancel());
    BOOST_CHECK_NO_THROW([&issuer_] {
        try { issuer_.set(42); }
        catch (target_::exceptions::BadState const &) { return; }
        throw ::std::logic_error{"unexpected state"};
    } ());
    BOOST_CHECK(! issuer_.exception(::std::runtime_error{"test"}));
    issuer_ = {};
    BOOST_CHECK(issuer_.cancel());
    BOOST_CHECK(static_cast<bool>(issuer_));
    BOOST_CHECK(::std::decay_t<decltype(state_)>::Cancelled == issuer_.state());
    BOOST_CHECK_NO_THROW([&issuer_] {
        try { issuer_.get(::std::rethrow_exception); }
        catch (target_::exceptions::Cancelled const &) { return; }
        throw ::std::logic_error{"unexpected state"};
    } ());
    BOOST_CHECK(! issuer_.cancel());
    BOOST_CHECK_NO_THROW([&issuer_] {
        try { issuer_.set(42); }
        catch (target_::exceptions::Cancelled const &) { return; }
        throw ::std::logic_error{"unexpected state"};
    } ());
    BOOST_CHECK(! issuer_.exception(::std::runtime_error{"test"}));

    [issuer_ = target_::future::Issuer<void>{}] () mutable {
        auto const front_ = issuer_.front();
        BOOST_CHECK(! front_.get());
        BOOST_CHECK(! static_cast<bool>(front_));
        BOOST_CHECK(! static_cast<bool>(front_.exception()));
        auto int_issuer_ = target_::future::Issuer<int>(::std::move(issuer_));
        int_issuer_.set(42);
        BOOST_CHECK(front_.get());
        BOOST_CHECK(! front_.cancel());
        BOOST_CHECK(static_cast<bool>(front_));
        BOOST_CHECK(! static_cast<bool>(front_.exception()));
    } ();
}

BOOST_AUTO_TEST_CASE(channel_case) {
    target_::channel::Issuer<int> issuer_;
    auto &front_ = issuer_.front();
    static_assert(::std::is_same_v<
        target_::Channel<int> const,
        ::std::remove_reference_t<decltype(front_)>
    >);
    static_assert(::std::is_same_v<
        target_::channel::Front<int> const,
        ::std::remove_reference_t<decltype(front_)>
    >);
    auto value_ = static_cast<int>(42);
    auto subscription_ = front_.subscribe([&value_] (auto &value) {
        static_assert(
            ::std::is_const_v<::std::remove_reference_t<decltype(value)>
        >);
        value_ = value;
    });
    BOOST_CHECK(static_cast<bool>(subscription_));
    if constexpr (true) {
        ::std::optional<target_::Subscription> subscription_ = ::std::nullopt;
        subscription_.emplace(front_.subscribe([
            &value_, &subscription_
        ] (auto const &value) {
            BOOST_CHECK(subscription_ && *subscription_);
            subscription_.reset();
            value_ = 42 * value * value_;
        }));
        BOOST_CHECK(subscription_ && *subscription_);
        issuer_.dispatch(1);
        BOOST_CHECK(! subscription_);
        BOOST_CHECK_EQUAL(value_, 42);
        issuer_.dispatch(2);
        BOOST_CHECK_EQUAL(value_, 2);
    }
    issuer_.dispatch(3);
    BOOST_CHECK_EQUAL(value_, 3);
    issuer_.dispatch(4);
    BOOST_CHECK_EQUAL(value_, 4);
    subscription_.cancel();
    BOOST_CHECK(! static_cast<bool>(subscription_));
    issuer_.dispatch(0);
    BOOST_CHECK_EQUAL(value_, 4);
}

BOOST_AUTO_TEST_SUITE_END()
