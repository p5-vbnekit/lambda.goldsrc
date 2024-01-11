#include <list>
#include <memory>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

#include <p5/lambda/utils/event.hxx>


namespace p5::lambda::utils::event {
namespace subscription {
namespace storage {

struct Type;

namespace entry {

using Pointer = ::std::shared_ptr<this_::Type>;
using Storage = parent_::Type &;
using Adapter = parent_::parent_::Type;
using Handler = parent_::parent_::parent_::source::driver::Handler;

struct Type final {
    using Pointer = this_::Pointer;
    using Adapter = this_::Adapter;
    using Storage = this_::Storage;
    using Handler = this_::Handler;

    Handler handler;
    Storage &storage;

    static Pointer get(Adapter const &) noexcept(true);
};

} // namespace entry

struct Type final: ::std::list<this_::entry::Pointer> {};

} // namespace storage

struct Type::Private_ final {
    bool weak = true;
    ::std::weak_ptr<this_::storage::Entry> entry;

    ~Private_() noexcept(true);
};

using Storage = storage::Type;

} // namespace subscription

namespace dispatcher::driver_ {

struct Type::Private_ final {
    parent_::parent_::subscription::Storage storage = {};
};

} // namespace dispatcher::driver_

namespace source {
namespace base {

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace base

namespace driver {

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace driver
} // namespace source

namespace dispatcher::driver_ {

this_::Type::Type::~Type() noexcept(true) = default;

this_::Type::Type() noexcept(false): private_{
    new ::std::decay_t<decltype(*private_)>
} {}

void this_::Type::dispatch(Event const &event) noexcept(true) {
    auto const storage_ = private_->storage;
    for (auto const &pointer_: storage_) {
        if (! pointer_) continue;
        auto const &handler_ = pointer_->handler;
        if (! handler_) continue;
        handler_(event);
    }
}

this_::Subscription this_::Type::subscribe(Handler &&handler) noexcept(false) {
    if (! handler) throw ::std::invalid_argument{"empty handler"};
    auto &storage_ = private_->storage;
    storage_.emplace_back(new parent_::parent_::subscription::storage::Entry{
        ::std::move(handler), storage_
    });
    return Subscription{storage_.back()};
}

void this_::Type::unsubscribe(
    Subscription const &subscription
) noexcept(false) {
    auto const entry_ = subscription::storage::Entry::get(subscription);
    if (! entry_) throw ::std::invalid_argument{"expired subscription"};
    auto &storage_ = private_->storage;
    if (&(entry_->storage) != &storage_) throw ::std::invalid_argument{
        "alien subscription"
    };
    auto const iterator_ = ::std::find(
        storage_.begin(), storage_.end(), entry_
    );
    if (storage_.end() == iterator_) throw ::std::invalid_argument{
        "subscription not found"
    };
    storage_.erase(iterator_);
}

} // namespace dispatcher::driver_

namespace subscription {

bool this_::Type::state() const noexcept(true) {
    if (! private_) return false;
    return ! private_->entry.expired();
}

bool this_::Type::cancel() noexcept(true) {
    if (! private_) return false;
    return [private_ = ::std::move(private_)] {
        auto &weak_ = private_->entry;
        if (weak_.expired()) return false;
        auto const shared_ = weak_.lock();
        weak_.reset();
        auto &storage_ = shared_->storage;
        auto const iterator_ = ::std::find(
            storage_.begin(), storage_.end(), shared_
        );
        if (storage_.end() == iterator_) return false;
        storage_.erase(iterator_);
        return true;
    } ();
}

bool this_::Type::detach() noexcept(true) {
    if (! private_) return false;
    private_.reset();
    return true;
}

this_::Type && this_::Type::take() noexcept(true) {
    if (private_) private_->weak = false;
    return ::std::move(*this);
}

this_::Type::Type::~Type() noexcept(true) = default;

this_::Type::Type(Pointer &&entry) noexcept(false): private_{
    entry.expired() ? nullptr : new ::std::decay_t<
        decltype(*private_)
    >{true, ::std::move(entry)}
} {}

this_::Type::Private_::~Private_() noexcept(true) {
    if (weak || entry.expired()) return;
    auto const entry_ = entry.lock();
    auto &storage_ = entry_->storage;
    auto const iterator_ = ::std::find(
        storage_.begin(), storage_.end(), entry_
    );
    if (storage_.end() == iterator_) return;
    storage_.erase(iterator_);
}

namespace storage::entry {

inline this_::Pointer this_::Type::get(Adapter const &adapter) noexcept(true) {
    if (! adapter.private_) return nullptr;
    return adapter.private_->entry.lock();
}

} // namespace storage::entry
} // namespace subscription
} // namespace p5::lambda::utils::event
