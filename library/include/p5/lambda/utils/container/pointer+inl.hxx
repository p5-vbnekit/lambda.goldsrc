#pragma once

#include <memory>
#include <utility>
#include <optional>
#include <coroutine>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include "pointer.hxx"


namespace p5::lambda::utils::container::pointer {

template <class Interface> inline
auto this_::Type<Interface>::unlink() noexcept(true) {
    if (node_.expired()) return false;
    node_.reset();
    return true;
}

template <class Interface> inline
auto this_::Type<Interface>::linked() const noexcept(true) {
    return ! node_.expired();
}

template <class Interface> inline
auto this_::Type<Interface>::check(bool qualifiers) const noexcept(true) {
    auto const node_ = this->node_.lock();
    if (! node_) return false;
    auto const &payload_ = node_->payload();
    if (! payload_.pointer.get()) return false;
    return ! (qualifiers && (payload_.qualifiers & (
        ~parent_::driver::node::payload::qualifiers<Interface>()
    )).any());
}

#if defined(__GNUC__) && (12 < __GNUC__) && (! defined (__clang__))
// suppress false positive warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdangling-reference"
#endif
template <class Interface> inline
auto & this_::Type<Interface>::resolve() const noexcept(false) {
    auto const node_ = this->node_.lock();
    if (! node_) throw ::std::logic_error{"bad link"};
    auto const &payload_ = node_->payload();
    if (! payload_.pointer) throw ::std::logic_error{"empty pointer"};
    auto const qualifiers_ = payload_.qualifiers & (
        ~parent_::driver::node::payload::qualifiers<Interface>()
    );
    if (qualifiers_.any()) throw ::std::invalid_argument{[&qualifiers_] {
        if (! (
            qualifiers_ ^ ::std::decay_t<decltype(qualifiers_)>{0x1}
        ).any()) return "unsatisfied const qualifier";
        if (! (
            qualifiers_ ^ ::std::decay_t<decltype(qualifiers_)>{0x2}
        ).any()) return "unsatisfied volatile qualifier";
        return "unsatisfied qualifiers: const, volatile";
    } ()};
    return *((Interface *)(payload_.pointer.get()));
}
#if defined(__GNUC__) && (12 < __GNUC__) && (! defined (__clang__))
#pragma GCC diagnostic pop
#endif

template <class Interface> inline
auto this_::Type<Interface>::subscribe(auto &&handler) const noexcept(false) {
    auto const node_ = this->node_.lock();
    if (! node_) throw ::std::logic_error{"bad link"};
    return node_->subscribe(::std::function<void(void)>{
        ::std::forward<decltype(handler)>(handler)
    });
}

template <class Interface> inline
auto * this_::Type<Interface>::address() const noexcept(true) {
    return (Interface *)([this] () -> void * {
        auto const node_ = this->node_.lock();
        if (! node_) return nullptr;
        auto const &payload_ = node_->payload();
        if (payload_.pointer && (payload_.qualifiers & (
            ~parent_::driver::node::payload::qualifiers<Interface>()
        )).any()) return nullptr;
        return payload_.pointer.get();
    } ());
}

template <class Interface> inline
auto & this_::Type<Interface>::reference() const noexcept(true) {
    return *(address());
}

template <class Interface> inline
this_::Type<Interface>::operator bool () const noexcept(true) {
    return check(true);
}

template <class Interface> inline
auto & this_::Type<Interface>::operator * () const noexcept(true) {
    return reference();
}

template <class Interface> inline
auto * this_::Type<Interface>::operator -> () const noexcept(true) {
    return address();
}

template <class Interface> inline
auto this_::Type<Interface>::operator co_await() const noexcept(true) {
    using Instance_ = ::std::decay_t<decltype(*this)>;
    using Subscription_ = ::std::optional<
        ::std::decay_t<decltype(subscribe([] {}))
    >>;

    struct Context_ final {
        Instance_ instance;
        Subscription_ subscription;
    };

    struct Result_ final {
        inline auto await_ready() const noexcept(true) {
            if (! context_.instance.linked()) return true;
            return context_.instance.check(false);
        }

        inline auto await_suspend(
            ::std::coroutine_handle<> const &coroutine
        ) const noexcept(false) {
            if (context_.subscription) throw ::std::logic_error{"bad state"};
            if (await_ready()) {
                context_.subscription.emplace();
                return false;
            }
            try {
                context_.subscription.emplace(context_.instance.subscribe([
                    this, coroutine
                ] {
                    if (! await_ready()) return;
                    context_.subscription.emplace();
                    coroutine.resume();
                }));
            } catch (...) {
                context_.instance.unlink();
                context_.subscription.emplace();
                throw;
            }
            return true;
        }

        inline auto & await_resume() const noexcept(false) {
            return context_.instance.resolve();
        }

        Result_(Result_ &&) noexcept(true) = default;

        inline explicit Result_(Instance_ const &instance) noexcept(true):
            context_{.instance{instance}, .subscription{::std::nullopt}}
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

template <class Interface> inline this_::Type<Interface>::Type(
    ::std::weak_ptr<Node const> &&node
) noexcept(true): node_{::std::move(node)} {}

} // namespace p5::lambda::utils::container::pointer
