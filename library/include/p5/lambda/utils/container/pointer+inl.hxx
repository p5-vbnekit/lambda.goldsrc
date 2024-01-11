#pragma once

#include <memory>
#include <utility>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include "pointer.hxx"
#include "../event/subscription.hxx"


namespace p5::lambda::utils::container::pointer {
namespace driver {

using Handler = ::std::function<void(void)>;
using Subscription = parent_::parent_::parent_::event::Subscription;

struct Type {
    using Handler = this_::Handler;
    using Subscription = this_::Subscription;

    virtual Subscription operator () (Handler &&) noexcept(false) = 0;

    virtual void * get() const noexcept(true) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true) = default;
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

private:
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace driver

#if defined(__GNUC__) && (13 < __GNUC__) && (! defined __clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdangling-reference"
#endif
template <class Interface> inline
auto & this_::Type<Interface>::resolve() const noexcept(false) {
    if (driver_.expired()) throw ::std::runtime_error{"expired"};
    auto * const pointer_ = driver_.lock()->get();
    if (! pointer_) { driver_.reset(); throw ::std::runtime_error{"expired"}; }
    return *static_cast<Interface *>(pointer_);
}
#if defined(__GNUC__) && (13 < __GNUC__) && (! defined __clang__)
#pragma GCC diagnostic pop
#endif

template <class Interface> inline
this_::Type<Interface>::operator bool () const noexcept(true) {
    if (driver_.expired()) return false;
    if (driver_.lock()->get()) return true;
    driver_.reset(); return false;
}

template <class Interface> inline
auto & this_::Type<Interface>::operator * () const noexcept(true) {
    return *static_cast<Interface *>([this] () -> void * {
        if (driver_.expired()) return nullptr;
        auto * const pointer_ = driver_.lock()->get();
        if (! pointer_) driver_.reset();
        return pointer_;
    });
}

template <class Interface> template <class Handler> inline
auto this_::Type<Interface>::operator () (
    Handler &&handler
) const noexcept(false) {
    using Handler_ = typename ::std::decay_t<
        decltype(*(driver_.lock()))
    >::Handler;
    auto const handler_ = Handler_{::std::forward<Handler>(handler)};
    if (! handler_) throw ::std::invalid_argument{"empty handler"};
    auto const shared_ = driver_.lock();
    if (shared_) {
        if (shared_->get()) return (*shared_)(::std::forward<Handler>(handler));
        else driver_.reset();
    }
    try { handler_(); } catch (...) {}
    return ::std::decay_t<decltype((*shared_)(::std::declval<Handler_>()))>{};
}

template <class Interface> inline this_::Type<Interface>::Type(
    ::std::shared_ptr<Driver> &&driver
) noexcept(true): driver_{[&driver] {
    if (driver && (! driver->get())) driver.reset();
    return ::std::move(driver);
} ()} {}

} // namespace p5::lambda::utils::container::pointer
