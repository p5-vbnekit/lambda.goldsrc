#pragma once

#include <tuple>
#include <utility>
#include <stdexcept>
#include <type_traits>

#include "type.hxx"
#include "driver.hxx"


namespace p5::lambda::utils::container {

inline auto this_::Type::unlink() noexcept(true) {
    if (! driver_) return false;
    driver_.reset();
    return true;
}

inline auto this_::Type::linked() const noexcept(true) {
    return static_cast<bool>(driver_);
}

template <class Interface> inline
auto this_::Type::get() const noexcept(false) {
    if (! driver_) throw ::std::logic_error{"bad link"};
    return Pointer<Interface>(driver_->node(
        this_::driver::node::key<Interface>()
    ));
}

template <class Interface> inline auto this_::Type::lock() noexcept(false) {
    if (! driver_) throw ::std::logic_error{"bad link"};
    return Lock<Interface>(driver_->node(
        this_::driver::node::key<Interface>()
    ));
}

template <class Interface> inline
auto this_::Type::assign(auto &&instance) noexcept(false) {
    using Reference_ = decltype(instance);
    using Instance_ = ::std::decay_t<decltype(*([] {
        static_assert(::std::is_reference_v<Reference_>);
        using Value_ = ::std::remove_reference_t<Reference_>;
        if constexpr (::std::is_rvalue_reference_v<Reference_>) {
            static_assert(::std::is_pointer_v<Value_>);
            return static_cast<Value_>(nullptr);
        } else return static_cast<Value_ *>(nullptr);
    } ()))>;
    auto lock_ = this->lock<::std::conditional_t<
        ::std::is_void_v<Interface>, Instance_, Interface
    >>();
    lock_.assign(instance);
    return lock_;
}

template <class Interface, class Instance> inline
auto this_::Type::emplace(auto && ... payload) noexcept(false) {
    using Instance_ = ::std::decay_t<decltype(*([] {
        if constexpr (::std::is_void_v<Instance>) {
            static_assert(::std::is_void_v<Interface>);
            static_assert(1 == sizeof ... (payload));
            return static_cast<::std::decay_t<::std::tuple_element_t<
                0, ::std::tuple<decltype(payload) ...>
            >> *>(nullptr);
        }
        else return static_cast<Instance *>(nullptr);
    }()))>;
    auto lock_ = this->lock<::std::conditional_t<
        ::std::is_void_v<Interface>, Instance_, Interface
    >>();
    lock_.template emplace<Instance_>(
        ::std::forward<decltype(payload)>(payload) ...
    );
    return lock_;
}

} // namespace p5::lambda::utils::container
