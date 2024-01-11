#include <list>
#include <memory>
#include <utility>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include <p5/lambda/utils/async.hxx>


namespace p5::lambda::utils::async {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

namespace subscription {

namespace parent_ = this_;
namespace this_ = parent_::subscription;

struct Driver;

struct Pool final {
    struct Item final {
        bool pinned = false;
        this_::Driver *driver;
        ::std::function<void(void const *)> handler;
    };

    bool lock = false;
    ::std::list<Item> list = {};
};

struct Driver final: parent_::parent_::subscription::Driver {
    struct Reference final {
        this_::Pool &pool;
        ::std::decay_t<decltype(::std::begin(pool.list))> iterator;
    };

    ::std::optional<Reference> reference = ::std::nullopt;

    virtual bool pin() noexcept(true) override final;
    virtual bool unpin() noexcept(true) override final;
    virtual bool pinned() const noexcept(true) override final;

    virtual bool state() const noexcept(true) override final;
    virtual bool cancel() noexcept(true) override final;

    Driver() noexcept(true) = default;
    virtual ~Driver() noexcept(true) override final;
};

} // namespace subscription

namespace channel {

namespace parent_ = this_;
namespace this_ = parent_::channel;

struct Driver final: parent_::parent_::channel::Driver {
    virtual bool dispatch(void const *) noexcept(true) override final;

    virtual Subscription subscribe(
        Handler &&
    ) noexcept(false) override final;

    Driver() noexcept(true) = default;
    virtual ~Driver() noexcept(true) override final;

private:
    parent_::subscription::Pool subscriptions_ = {};
};

} // namespace channel

namespace future {

namespace parent_ = this_;
namespace this_ = parent_::future;

struct Driver final: parent_::parent_::future::Driver {
    virtual void set(Pointer &&) noexcept(false) override final;

    virtual void * get() const noexcept(true) override final;
    virtual void * get_or_throw() const noexcept(false) override final;

    virtual State state() const noexcept(true) override final;

    virtual Exception exception() const noexcept(true) override final;
    virtual bool exception(Exception const &) noexcept(true) override final;

    virtual bool cancel() noexcept(true) override final;
    virtual Subscription subscribe(Handler &&) noexcept(false) override final;

    Driver() noexcept(true) = default;
    virtual ~Driver() noexcept(true) override final;

private:
    struct Context_ final {
        ::std::optional<
            parent_::parent_::channel::Issuer<void>
        > channel = ::std::nullopt;
        ::std::optional<Pointer> pointer = ::std::nullopt;
        bool cancelled = false;
        Exception exception = nullptr;
    } context_ = {};
};

} // namespace future
} // namespace private_

namespace future {
namespace front {

Base::Base::~Base() noexcept(true) = default;

Base::Base(Base &&) noexcept(true) = default;
Base::Base(Base const &) noexcept(true) = default;

Base & Base::operator = (Base &&) noexcept(true) = default;
Base & Base::operator = (Base const &) noexcept(true) = default;

Base::Base(::std::decay_t<decltype(driver_)> &&driver) noexcept(true):
    driver_{::std::move(driver)}
{}

} // namespace front

namespace issuer {

Base::Base::~Base() noexcept(true) = default;

Base::Base() noexcept(false):
    driver_{::std::make_shared<parent_::parent_::private_::future::Driver>()}
{}

Base::Base(Base &&) noexcept(true) = default;
Base & Base::operator = (Base &&) noexcept(true) = default;

} // namespace issuer

namespace driver {

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace driver
} // namespace future

namespace channel {
namespace front {

Base::Base::~Base() noexcept(true) = default;

Base::Base(Base &&) noexcept(true) = default;
Base::Base(Base const &) noexcept(true) = default;

Base & Base::operator = (Base &&) noexcept(true) = default;
Base & Base::operator = (Base const &) noexcept(true) = default;

Base::Base(::std::decay_t<decltype(driver_)> &&driver) noexcept(true):
    driver_{::std::move(driver)}
{}

} // namespace front

namespace issuer {

Base::Base::~Base() noexcept(true) = default;

Base::Base() noexcept(false):
    driver_{::std::make_shared<parent_::parent_::private_::channel::Driver>()}
{}

Base::Base(Base &&) noexcept(true) = default;
Base & Base::operator = (Base &&) noexcept(true) = default;

} // namespace issuer

namespace driver {

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace driver
} // namespace channel

namespace exceptions {

BadState::BadState(char const *text) noexcept(true):
    ::std::logic_error{text ? text : "bad state"}
{}

Cancelled::Cancelled(char const *text) noexcept(true):
    ::std::runtime_error{text ? text : "cancelled"}
{}

} // namespace exceptions

namespace subscription::driver {

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace subscription::driver

namespace private_ {
namespace future {

inline void this_::Driver::set(Pointer &&pointer) noexcept(false) {
    if (context_.cancelled) throw parent_::parent_::exceptions::Cancelled{};
    if (context_.exception) throw parent_::parent_::exceptions::BadState{
        "bad future state: exception assigned"
    };
    if (context_.pointer) throw parent_::parent_::exceptions::BadState{
        "bad future state: value assigned"
    };
    context_.pointer.emplace(::std::move(pointer));
    if (context_.channel) ::std::exchange(
        context_.channel, ::std::nullopt
    )->dispatch();
}

inline void * this_::Driver::get() const noexcept(true) {
    if (context_.cancelled) return nullptr;
    if (context_.exception) return nullptr;
    if (! context_.pointer) return nullptr;
    return context_.pointer->get();
}

inline void * this_::Driver::get_or_throw() const noexcept(false) {
    if (context_.exception) {
        ::std::rethrow_exception(context_.exception);
        if (context_.cancelled) throw parent_::parent_::exceptions::Cancelled{};
        throw parent_::parent_::exceptions::BadState{"unknown error"};
    }
    if (context_.cancelled) throw parent_::parent_::exceptions::Cancelled{};
    if (! context_.pointer) throw parent_::parent_::exceptions::BadState{
        "bad future state: not ready"
    };
    return context_.pointer->get();
}

inline this_::Driver::State this_::Driver::state() const noexcept(true) {
    if (context_.cancelled) return State::Cancelled;
    if (context_.exception || context_.pointer) return State::Ready;
    return State::Initial;
}

inline this_::Driver::Exception
this_::Driver::exception() const noexcept(true) { return context_.exception; }

inline bool this_::Driver::exception(
    Exception const &exception
) noexcept(true) {
    if (! exception) return false;
    if (context_.cancelled) return false;
    if (context_.exception) return false;
    if (context_.pointer) return false;
    context_.exception = exception;
    if (context_.channel) ::std::exchange(
        context_.channel, ::std::nullopt
    )->dispatch();
    return true;
}

inline bool this_::Driver::cancel() noexcept(true) {
    if (context_.cancelled) return false;
    if (context_.exception) return false;
    if (context_.pointer) return false;
    context_.cancelled = true;
    context_.exception = ::std::make_exception_ptr(
        parent_::parent_::exceptions::Cancelled{}
    );
    if (context_.channel) ::std::exchange(
        context_.channel, ::std::nullopt
    )->dispatch();
    return true;
}

inline this_::Driver::Subscription this_::Driver::subscribe(
    Handler &&handler
) noexcept(false) {
    if (! handler) throw ::std::invalid_argument{"empty handler"};
    if (context_.cancelled || context_.exception || context_.pointer) {
        try { handler(); } catch(...) {}; return {};
    }
    if (! context_.channel) context_.channel.emplace();
    return context_.channel->subscribe(::std::move(handler));
}

inline this_::Driver::Driver::~Driver() noexcept(true) {
    if (context_.cancelled) return;
    if (context_.exception) return;
    if (! context_.channel) return;
    if (context_.pointer) return;
    context_.cancelled = true;
    context_.exception = ::std::make_exception_ptr(
        parent_::parent_::exceptions::Cancelled{}
    );
    ::std::exchange(context_.channel, ::std::nullopt)->dispatch();
}

} // namespace future

namespace channel {

inline bool this_::Driver::dispatch(void const *event) noexcept(true) {
    auto iterator_ = ::std::begin(subscriptions_.list);
    if (::std::end(subscriptions_.list) == iterator_) return false;

    auto const lock_ = ! subscriptions_.lock;
    if (lock_) subscriptions_.lock = true;

    do {
        auto const &item_ = *iterator_;
        if (item_.pinned || item_.driver) {
            try { item_.handler(event); }
            catch (...) {}
            ++iterator_;
        }
        else if (lock_) subscriptions_.list.erase(iterator_++);
    } while (::std::end(subscriptions_.list) != iterator_);

    if (lock_) subscriptions_.lock = false;
    return true;
}

inline this_::Driver::Subscription this_::Driver::subscribe(
    Handler &&handler
) noexcept(false) {
    if (! handler) throw ::std::invalid_argument{"empty handler"};
    auto subscription_ = ::std::make_shared<parent_::subscription::Driver>();
    auto &&iterator_ = subscriptions_.list.insert(
        ::std::end(subscriptions_.list),
        {.driver = subscription_.get(), .handler = ::std::move(handler)}
    );
    subscription_->reference.emplace(::std::decay_t<
        decltype(*(subscription_->reference))
    >{.pool = subscriptions_, .iterator = ::std::move(iterator_)});
    return this_::Driver::Subscription{::std::move(subscription_)};
}

inline this_::Driver::Driver::~Driver() noexcept(true) {
    for (auto &item_: subscriptions_.list) {
        if (! item_.driver) continue;
        item_.driver->reference = ::std::nullopt;
    }
}

} // namespace channel

namespace subscription {

inline bool this_::Driver::pin() noexcept(true) {
    return reference && (! ::std::exchange(reference->iterator->pinned, true));
}

inline bool this_::Driver::unpin() noexcept(true) {
    return reference && ::std::exchange(reference->iterator->pinned, false);
}

inline bool this_::Driver::pinned() const noexcept(true) {
    return reference && reference->iterator->pinned;
}

inline bool this_::Driver::state() const noexcept(true) {
    return static_cast<bool>(reference);
}

inline bool this_::Driver::cancel() noexcept(true) {
    if (! reference) return false;
    auto const reference_ = ::std::exchange(reference, ::std::nullopt);
    if (reference_->pool.lock) {
        auto &item_ = *(reference_->iterator);
        item_.pinned = false;
        item_.driver = nullptr;
    }
    else reference_->pool.list.erase(reference_->iterator);
    return true;
}

inline this_::Driver::Driver::~Driver() noexcept(true) {
    if (! reference) return;
    auto const reference_ = ::std::exchange(reference, ::std::nullopt);
    if (
        reference_->pool.lock || reference_->iterator->pinned
    ) reference_->iterator->driver = nullptr;
    else reference_->pool.list.erase(reference_->iterator);
}

} // namespace subscription
} // namespace private_
} // namespace p5::lambda::utils::async
