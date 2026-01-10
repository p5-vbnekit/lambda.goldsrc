#pragma once

#include <utility>
#include <optional>
#include <exception>
#include <stdexcept>
#include <coroutine>
#include <functional>
#include <type_traits>

#include "front.hxx"
#include "exceptions.hxx"


namespace p5::lambda::utils::async::future::front {

template <class T> inline auto this_::Type<T>::unlink() noexcept(true) {
    auto &driver_ = this_::Base::driver_;
    if (driver_.expired()) return false;
    driver_.reset();
    return true;
}

template <class T> inline auto this_::Type<T>::linked() const noexcept(true) {
    return ! this_::Base::driver_.expired();
}

template <class T> inline
auto this_::Type<T>::get() const noexcept(true) {
    auto * const pointer_ = [
        driver_ = this_::Base::driver_.lock()
    ] () -> Value * {
        if (! driver_) return nullptr;
        return static_cast<Value *>(driver_->get());
    } ();
    if constexpr (::std::is_void_v<Value>) return static_cast<bool>(pointer_);
    else return pointer_;
}

template <class T> inline
auto this_::Type<T>::get(auto &&exception_handler) const noexcept(false) {
    auto exception_ = ::std::exception_ptr{};

    try {
        auto const driver_ = this_::Base::driver_.lock();
        if (! driver_) throw parent_::exceptions::BadState{
            "bad future state: empty driver pointer"
        };
        auto * const pointer_ = driver_->get_or_throw();
        if (! pointer_) throw parent_::exceptions::BadState{
            "bad future state: empty value pointer"
        };
        if constexpr (::std::is_void_v<Value>) return true;
        else return static_cast<Value *>(pointer_);
    }

    catch (...) { exception_ = ::std::current_exception(); }

    if (auto const handler_ = ::std::function<
        void(::std::exception_ptr const &)
    >{
        ::std::forward<decltype(exception_handler)>(exception_handler)
    }) {
        if (! exception_) {
            exception_ = ::std::make_exception_ptr(::std::bad_exception{});
            if (! exception_) throw ::std::bad_exception{};
        }
        handler_(exception_);
    }

    else {
        if (! exception_) throw ::std::bad_exception{};
        ::std::rethrow_exception(exception_);
        throw parent_::exceptions::BadState{};
    }

    if constexpr (::std::is_void_v<Value>) return false;
    else return static_cast<Value *>(nullptr);
}

template <class T> inline
auto this_::Type<T>::state() const noexcept(true) {
    auto const driver_ = this_::Base::driver_.lock();
    if (driver_) return driver_->state();
    return ::std::decay_t<decltype(driver_->state())>::Bad;
}

template <class T> inline
auto this_::Type<T>::exception() const noexcept(true) {
    auto const driver_ = this_::Base::driver_.lock();
    if (driver_) return driver_->exception();
    return ::std::decay_t<decltype(driver_->exception())>{};
}

template <class T> inline
auto this_::Type<T>::cancel() const noexcept(true) {
    auto const driver_ = this_::Base::driver_.lock();
    return driver_ && driver_->cancel();
}

template <class T> inline
auto this_::Type<T>::subscribe(auto &&handler) const noexcept(false) {
    auto const driver_ = this_::Base::driver_.lock();
    if (! driver_) throw parent_::exceptions::BadState{
        "bad future state: empty driver pointer"
    };
    auto &&handler_ = Handler{::std::forward<decltype(handler)>(handler)};
    if (! handler_) throw ::std::invalid_argument{"empty handler"};
    return driver_->subscribe(::std::move(handler_));
}

template <class T> inline
this_::Type<T>::operator bool () const noexcept(true) {
    return ::std::decay_t<decltype(state())>::Initial != state();
}

template <class T> inline
auto & this_::Type<T>::operator * () const noexcept(true) requires(
    ! ::std::is_void_v<this_::Value<T>>
) { return *(this->get()); }

template <class T> inline
auto * this_::Type<T>::operator -> () const noexcept(true) requires(
    ! ::std::is_void_v<this_::Value<T>>
) { return this->get(); }

template <class T> inline
auto this_::Type<T>::operator co_await() const noexcept(true) {
    using Instance_ = ::std::decay_t<decltype(*this)>;
    using Subscription_ = ::std::optional<Subscription>;

    struct Context_ final {
        Instance_ instance{};
        Subscription_ subscription{::std::nullopt};
    };

    struct Result_ final {
        inline auto await_ready() const noexcept(true) {
            return static_cast<bool>(context_.instance);
        }

        inline auto await_suspend(
            ::std::coroutine_handle<> coroutine
        ) const noexcept(false) {
            if (context_.subscription) throw parent_::exceptions::BadState{};
            if (context_.instance) throw parent_::exceptions::BadState{};
            try { context_.subscription.emplace(context_.instance.subscribe(
                [coroutine] { coroutine.resume(); })
            ); } catch (...) { context_ = {}; throw; }
        }

        inline decltype(auto) await_resume() const noexcept(false) {
            ::std::decay_t<decltype(
                context_.instance.get(::std::rethrow_exception))
            > result_;
            try {
                if (! context_.subscription) context_.subscription.emplace();
                if (! context_.instance) throw parent_::exceptions::BadState{};
                result_ = context_.instance.get(::std::rethrow_exception);
                if (! result_) throw parent_::exceptions::BadState{};
            }
            catch (...) { context_ = {}; throw; }
            if constexpr (! ::std::is_void_v<Value>) return *result_;
        }

        Result_(Result_ &&) noexcept(true) = default;

        inline explicit Result_(Instance_ const &instance) noexcept(true):
            context_{.instance{instance}}
        {}

    private:
        mutable Context_ context_;

        Result_() = delete;
        Result_(Result_ const &) = delete;
        Result_ & operator = (Result_ &&) = delete;
        Result_ & operator = (Result_ const &) = delete;
    };

    return Result_{*this};
}

template <class T> inline this_::Type<T>::Type(
    ::std::decay_t<decltype(driver_)> &&driver
) noexcept(true): this_::Base{::std::move(driver)} {}

template <class T> template <class O> inline this_::Type<T>::Type(
    this_::Type<O> &&other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
): this_::Base{::std::move(static_cast<this_::Base &>(other))} {}

template <class T> template <class O> inline this_::Type<T>::Type(
    this_::Type<O> const &other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
): this_::Base{other} {}

template <class T> template <class O> inline auto & this_::Type<T>::operator = (
    this_::Type<O> &&other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
) {
    static_cast<this_::Base *>(this)->operator = (::std::move(other));
    return *this;
}

template <class T> template <class O> inline auto & this_::Type<T>::operator = (
    this_::Type<O> const &other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
) {
    static_cast<this_::Base *>(this)->operator = (other);
    return *this;
}

} // namespace p5::lambda::utils::async::future::front
