#pragma once

#include <memory>
#include <utility>
#include <stdexcept>
#include <type_traits>

#include "lock.hxx"


namespace p5::lambda::utils::container::lock {

template <class Interface> inline
auto this_::Type<Interface>::unlink() noexcept(true) {
    auto const node_ = this->node_.lock();
    if (! node_) return false;
    this->node_.reset();
    if (pinned_) return true;
    node_->lock(false);
    node_->payload({.pointer = {nullptr, nullptr}, .qualifiers = 0});
    return true;
}

template <class Interface> inline
auto this_::Type<Interface>::linked() const noexcept(true) {
    return ! node_.expired();
}

template <class Interface> inline
auto this_::Type<Interface>::pin() noexcept(true) {
    return ! (node_.expired() || ::std::exchange(pinned_, true));
}

template <class Interface> inline
auto this_::Type<Interface>::unpin() noexcept(true) {
    return (! node_.expired()) && ::std::exchange(pinned_, false);
}

template <class Interface> inline
auto this_::Type<Interface>::pinned() const noexcept(true) {
    return (! node_.expired()) && pinned_;
}

template <class Interface> inline
auto this_::Type<Interface>::release() noexcept(true) {
    auto const node_ = this->node_.lock();
    if (! node_) return false;
    this->node_.reset();
    node_->lock(false);
    node_->payload({.pointer = {nullptr, nullptr}, .qualifiers = 0});
    return true;
}

template <class Interface> inline
auto this_::Type<Interface>::assign(auto *instance) noexcept(false) {
    using Instance_ = ::std::remove_reference_t<decltype(*instance)>;
    static_assert(::std::is_base_of_v<Interface, Instance_>);
    static_assert(::std::is_convertible_v<Instance_ *, Interface *>);
    auto const node_ = this->node_.lock();
    if (! node_) throw ::std::logic_error{"bad link"};
    node_->payload({
        .pointer = {(void *)instance, [] (auto const *) {}},
        .qualifiers = parent_::driver::node::payload::qualifiers<Interface>()
    });
}

template <class Interface> inline
auto this_::Type<Interface>::assign(auto &instance) noexcept(false) {
    return this->assign(&instance);
}

template <class Interface> template <class Instance> inline
auto this_::Type<Interface>::emplace(auto && ... payload) noexcept(false) {
    static_assert(::std::is_base_of_v<Interface, Instance>);
    static_assert(::std::is_convertible_v<Instance *, Interface *>);
    auto const node_ = this->node_.lock();
    if (! node_) throw ::std::logic_error{"bad link"};
    node_->payload({
        .pointer = {
            new Instance{::std::forward<decltype(payload)>(payload) ...},
            [] (auto const *pointer) { delete ((Instance *)pointer); }
        },
        .qualifiers = parent_::driver::node::payload::qualifiers<Interface>()
    });
}

template <class Interface> inline
this_::Type<Interface>::Type() noexcept(true) = default;

template <class Interface> inline
this_::Type<Interface>::Type(Type &&) noexcept(true) = default;

template <class Interface> inline
auto & this_::Type<Interface>::operator = (Type &&other) noexcept(true) {
    if (this != &other) {
        auto const node_ = this->node_.lock();
        auto const pinned_ = this->pinned_;
        this->node_ = ::std::exchange(other.node_, {});
        this->pinned_ = ::std::exchange(other.pinned_, false);
        if (node_ && (! pinned_)) {
            node_->lock(false);
            node_->payload({});
        }
    }
    return *this;
}

template <class Interface> inline
this_::Type<Interface>::Type::~Type() noexcept(true) {
    unlink();
}

template <class Interface> inline
this_::Type<Interface>::Type(
    ::std::shared_ptr<Node> const &node
) noexcept(false) {
    if (! node) return;
    if (! node->lock(true)) throw ::std::logic_error{"resource busy"};
    this->node_ = node;
}

} // namespace p5::lambda::utils::container::lock
