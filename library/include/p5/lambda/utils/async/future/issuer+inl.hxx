#pragma once

#include <memory>
#include <utility>
#include <exception>
#include <type_traits>

#include "issuer.hxx"
#include "exceptions.hxx"


namespace p5::lambda::utils::async::future::issuer {

inline auto this_::Type<void>::unlink() noexcept(true) {
    auto &driver_ = this_::Base::driver_;
    if (! driver_) return false;
    driver_.reset();
    return true;
}

inline auto this_::Type<void>::linked() const noexcept(true) {
    return static_cast<bool>(this_::Base::driver_);
}

inline auto const & this_::Type<void>::front() const noexcept(true) {
    return front_;
}

inline auto this_::Type<void>::get() const noexcept(true) {
    return front_.get();
}

inline auto this_::Type<void>::get(auto &&handler) const noexcept(false) {
    return front_.get(::std::forward<decltype(handler)>(handler));
}

inline auto this_::Type<void>::set() noexcept(false) {
    auto const &driver_ = this_::Base::driver_;
    if (! driver_) throw parent_::exceptions::BadState{
        "bad future state: empty driver pointer"
    };
    driver_->set({driver_.get(), [] (auto const *) {}});
}

inline auto this_::Type<void>::state() const noexcept(true) {
    return front_.state();
}

inline auto this_::Type<void>::cancel() const noexcept(true) {
    return front_.cancel();
}

inline auto this_::Type<void>::subscribe(auto &&handler) const noexcept(false) {
    return front_.subscribe(::std::forward<decltype(handler)>(handler));
}

inline auto this_::Type<void>::exception() const noexcept(true) {
    return front_.exception();
}

inline auto this_::Type<void>::exception(auto &&exception) noexcept(true) {
    auto const &driver_ = this_::Base::driver_;
    return driver_ && driver_->exception([&exception_ = exception] {
        if constexpr (::std::is_base_of_v<
            ::std::decay_t<decltype(this_::Base::driver_->exception())>,
            ::std::decay_t<decltype(exception)>>
        ) return ::std::forward<decltype(exception)>(exception_);
        else return ::std::make_exception_ptr(
            ::std::forward<decltype(exception)>(exception_)
        );
    } ());
}

inline this_::Type<void>::operator bool () const noexcept(true) {
    return front_.operator bool();
}

inline this_::Type<void>::operator this_::Front<void> () const noexcept(true) {
    return front_;
}

inline auto this_::Type<void>::operator co_await() const noexcept(true) {
    return front_.operator co_await();
}

inline this_::Type<void>::Type() noexcept(false):
    this_::Base{}, front_{this_::Base::driver_}
{}

template <class T> inline auto this_::Type<T>::unlink() noexcept(true) {
    auto &driver_ = this_::Base::driver_;
    if (! driver_) return false;
    driver_.reset();
    return true;
}

template <class T> inline auto this_::Type<T>::linked() const noexcept(true) {
    return static_cast<bool>(this_::Base::driver_);
}

template <class T> inline
auto const & this_::Type<T>::front() const noexcept(true) { return front_; }

template <class T> inline auto this_::Type<T>::get() const noexcept(true) {
    return front_.get();
}

template <class T> inline
auto this_::Type<T>::get(auto &&handler) const noexcept(false) {
    return front_.get(::std::forward<decltype(handler)>(handler));
}

template <class T> inline
auto this_::Type<T>::set(auto &&reference) noexcept(false) {
    if (! this_::Base::driver_) throw parent_::exceptions::BadState{
        "bad future state: empty driver pointer"
    };
    this_::Base::driver_->set([&reference] () {
        using Pointer_ = typename ::std::decay_t<
            decltype(*(this_::Base::driver_))
        >::Pointer;
        using Reference_ = decltype(reference);
        static_assert(::std::is_reference_v<Reference_>);
        if constexpr (::std::is_reference_v<T>) {
            static_assert(::std::is_convertible_v<Reference_, Value &>);
            return Pointer_{(void *)(&reference), [] (auto const *) {}};
        }
        else return Pointer_{
            (void *)(new Value{::std::forward<Reference_>(reference)}),
            [] (auto const *p) { delete (Value *)p; }
        };
    } ());
}

template <class T> inline auto this_::Type<T>::emplace(
    auto && ... payload
) noexcept(false) requires(! ::std::is_reference_v<T>) {
    if (! this_::Base::driver_) throw parent_::exceptions::BadState{
        "bad future state: empty driver pointer"
    };
    this_::Base::driver_->set(typename ::std::decay_t<
        decltype(*(this_::Base::driver_))
    >::Pointer{
        (void *)(new Value{::std::forward<decltype(payload)>(payload) ...}),
        [] (auto const *p) { delete (Value *)p; }
    });
}

template <class T> inline auto this_::Type<T>::state() const noexcept(true) {
    return front_.state();
}

template <class T> inline auto this_::Type<T>::cancel() const noexcept(true) {
    return front_.cancel();
}

template <class T> inline
auto this_::Type<T>::subscribe(auto &&handler) const noexcept(false) {
    return front_.subscribe(::std::forward<decltype(handler)>(handler));
}

template <class T> inline
auto this_::Type<T>::exception() const noexcept(true) {
    return front_.exception();
}

template <class T> inline
auto this_::Type<T>::exception(auto &&exception) noexcept(true) {
    auto const &driver_ = this_::Base::driver_;
    return driver_ && driver_->exception([&exception_ = exception] {
        if constexpr (::std::is_base_of_v<
            ::std::decay_t<decltype(this_::Base::driver_->exception())>,
            ::std::decay_t<decltype(exception)>>
        ) return ::std::forward<decltype(exception)>(exception_);
        else return ::std::make_exception_ptr(
            ::std::forward<decltype(exception)>(exception_)
        );
    } ());
}

template <class T> inline
this_::Type<T>::operator bool () const noexcept(true) {
    return front_.operator bool();
}

template <class T> inline
this_::Type<T>::operator this_::Front<T> () const noexcept(true) {
    return front_;
}

template <class T> inline
auto & this_::Type<T>::operator * () const noexcept(true) {
    return front_.operator *();
}

template <class T> inline
auto this_::Type<T>::operator co_await() const noexcept(true) {
    return front_.operator co_await();
}

template <class T> inline this_::Type<T>::Type() noexcept(false):
    this_::Base{}, front_{this_::Base::driver_}
{}

template <class T> template <class O> inline this_::Type<T>::Type(
    this_::Type<O> &&other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
):
    this_::Base{::std::move(static_cast<this_::Base &>(other))},
    front_{this_::Base::driver_}
{}

template <class T> template <class O> inline auto & this_::Type<T>::operator = (
    this_::Type<O> &&other
) noexcept(true) requires(
    this_::is_convertible<O, T>() && (! ::std::is_same_v<T, O>)
) {
    static_cast<this_::Base *>(this)->operator = (::std::exchange(
        other, ::std::decay_t<decltype(other)>{this_::Base::driver_}
    ));
    front_ = ::std::decay_t<decltype(front_)>{this_::Base::driver_};
    return *this;
}

} // namespace p5::lambda::utils::async::future::issuer
