#pragma once

#include <tuple>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <exception>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include "type.hxx"
#include "lock.hxx"
#include "driver.hxx"
#include "pointer.hxx"
#include "../event/subscription.hxx"


namespace p5::lambda::utils::container {
namespace type {

namespace parent_ = this_;
namespace this_ = parent_::type;

template <class T> inline constexpr static auto key() noexcept(true) {
    static_assert(::std::is_class_v<T>);
    return ::std::type_index{typeid(::std::remove_cv_t<T>)};
}

template <class T> inline constexpr static auto qualifiers() noexcept(true) {
    static_assert(::std::is_class_v<T>);
    return parent_::driver::Qualifiers{(
        ::std::is_const_v<T> ? 0x1 : 0x00
    ) | (
        ::std::is_volatile_v<T> ? 0x2 : 0x00
    )};
}

} // namespace type

template <class Interface> inline
auto this_::Type::get() const noexcept(true) {
    auto const key_ = this_::type::key<Interface>();
    auto const qualifiers_ = this_::type::qualifiers<Interface>();
    auto pointer_ = ::std::decay_t<decltype(driver_->get(key_, qualifiers_))>{};
    if (driver_) try {
        pointer_ = driver_->get(key_, qualifiers_);
    } catch (...) {}
    return Pointer<Interface>(::std::move(pointer_));
}

template <class Interface, class Handler> inline
auto this_::Type::get(Handler &&handler) const noexcept(false) {
    auto const key_ = this_::type::key<Interface>();
    auto const qualifiers_ = this_::type::qualifiers<Interface>();
    auto pointer_ = ::std::decay_t<decltype(driver_->get(key_, qualifiers_))>{};
    auto handler_ = ::std::function<void(::std::exception_ptr const &)>{
        ::std::forward<Handler>(handler)
    };
    if (driver_) try {
        pointer_ = driver_->get(key_, qualifiers_);
    } catch (...) { if (handler_) handler_(::std::current_exception()); }
    return Pointer<Interface>(::std::move(pointer_));
}

template <class Interface> inline auto this_::Type::lock() noexcept(false) {
    if (! driver_) throw ::std::logic_error{"expired"};
    return Lock<Interface>{driver_->lock(
        this_::type::key<Interface>(), this_::type::qualifiers<Interface>()
    )};
}

template <class Interface, class Factory> inline
auto this_::Type::inject(Factory &&factory) noexcept(false) {
    auto lock_ = this->lock<::std::conditional_t<
        ::std::is_void_v<Interface>,
        ::std::remove_pointer_t<::std::remove_reference_t<decltype(factory())>>,
        Interface
    >>();
    lock_.inject(::std::forward<Factory>(factory));
    return lock_;
}

template <class Interface, class Instance, class Factory> inline
auto this_::Type::inject(
    ::std::in_place_t const &in_place, Factory &&factory
) noexcept(false) {
    auto lock_ = this->lock<Interface>();
    lock_.template inject<Instance>(in_place, ::std::forward<Factory>(factory));
    return lock_;
}

template <class Interface, class Instance, class Factory> inline
auto this_::Type::inject(
    ::std::in_place_type_t<Instance> const &in_place, Factory &&factory
) noexcept(false) {
    auto lock_ = this->lock<::std::conditional_t<
        ::std::is_void_v<Interface>, Instance, Interface
    >>();
    lock_.inject(in_place, ::std::forward<Factory>(factory));
    return lock_;
}

template <class Interface, class Instance> inline
auto this_::Type::assign(Instance &&instance) noexcept(false) {
    auto lock_ = this->lock<::std::conditional_t<
        ::std::is_void_v<Interface>,
        ::std::remove_pointer_t<::std::remove_reference_t<Instance>>,
        Interface
    >>();
    lock_.assign(::std::forward<Instance>(instance));
    return lock_;
}

template <class Interface, class Instance, class ... T> inline
auto this_::Type::emplace(T && ... payload) noexcept(false) {
    if constexpr (::std::is_void_v<Instance>) {
        static_assert(::std::is_void_v<Interface>);
        static_assert(1 == sizeof ... (T));
    }
    using Instance_ = ::std::decay_t<decltype(*[] {
        if constexpr (::std::is_void_v<Instance>) return static_cast<
            ::std::decay_t<::std::tuple_element_t<0, ::std::tuple<T ...>>> *
        >(nullptr);
        else return static_cast<Instance *>(nullptr);
    }())>;
    auto lock_ = this->lock<::std::conditional_t<
        ::std::is_void_v<Interface>, Instance_, Interface
    >>();
    lock_.template emplace<Instance_>(::std::forward<T>(payload) ...);
    return lock_;
}

inline auto this_::Type::collect() noexcept(true) {
    if (driver_) driver_->collect();
}

template <class Handler> inline
auto this_::Type::on_error(Handler &&handler) const noexcept(false) {
    if (! driver_) throw ::std::logic_error{"expired"};
    auto handler_ = ::std::function<void(
        ::std::type_index const &, ::std::exception_ptr const &
    )>{::std::forward<Handler>(handler)};
    if (! handler_) throw ::std::invalid_argument{"empty handler"};
    return driver_->on_error(::std::move(handler));
}

template <class Interface, class Handler> inline
auto this_::Type::on_error(Handler &&handler) const noexcept(false) {
    static_assert(::std::is_same_v<::std::decay_t<Interface>, Interface>);
    if (! driver_) throw ::std::logic_error{"expired"};
    auto handler_ = ::std::function<
        void(::std::exception_ptr const &)
    >{::std::forward<Handler>(handler)};
    if (! handler_) throw ::std::invalid_argument{"empty handler"};
    return driver_->on_error([
        key_ = this_::type::key<Interface>(),
        handler_ = ::std::move(handler)
    ] (auto const &key, auto const &exception) {
        if (key_ == key) return handler_(exception);
    });
}

} // namespace p5::lambda::utils::container
