#include <memory>
#include <variant>
#include <iterator>
#include <optional>
#include <typeindex>
#include <exception>
#include <functional>
#include <unordered_map>

#include <p5/lambda/utils/container.hxx>
#include <p5/lambda/utils/event/dispatcher.hxx>


namespace p5::lambda::utils::container {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

namespace storage {

namespace parent_ = this_;
namespace this_ = parent_::storage;

using Key = ::std::type_index;
using Holder = parent_::parent_::lock::Driver::Holder;
using Pointer = parent_::parent_::pointer::Driver;
using Qualifiers = parent_::parent_::driver::Qualifiers;

struct Item final {
    using Holder = this_::Holder;
    using Pointer = this_::Pointer;
    using Qualifiers = this_::Qualifiers;

    ::std::shared_ptr<Pointer> pointer = {};
    ::std::variant<
        ::std::monostate,
        ::std::exception_ptr,
        ::std::unique_ptr<Holder>,
        ::std::function<void *(void)>,
        ::std::function<::std::unique_ptr<Holder>(void)>
    > payload = {};
    Qualifiers qualifiers = {};
};

using Type = ::std::unordered_map<Key, Item>;

auto invoke_factory(Item &) noexcept(false);

} // namespace storage

using Storage = storage::Type;

struct Lock final: parent_::lock::Driver {
    virtual void put(::std::variant<
        void *, ::std::unique_ptr<Holder>,
        ::std::function<void *(void)>,
        ::std::function<::std::unique_ptr<Holder>(void)>
    > &&) noexcept(false) override final;

    virtual void take() noexcept(true) override final;

    Lock(::std::type_index const &key, this_::Storage &storage) noexcept(true);

    ~Lock() noexcept(true);

private:
    struct Private_ final {
        ::std::type_index key;
        bool weak = true;
        this_::Storage &storage;
    } private_;

    Lock() = delete;
    Lock(Lock &&) = delete;
    Lock(Lock const &) = delete;
    Lock & operator = (Lock &&) = delete;
    Lock & operator = (Lock const &) = delete;
};

struct Driver final: parent_::Driver {
    virtual ::std::shared_ptr<Pointer> get(
        ::std::type_index const &, Qualifiers const &
    ) const noexcept(false) override final;

    virtual ::std::unique_ptr<Lock> lock(
        ::std::type_index &&, Qualifiers &&
    ) noexcept(false) override final;

    virtual void collect() noexcept(true) override final;

    virtual Subscription on_error(::std::function<
        void(::std::type_index const &, ::std::exception_ptr const &)
    > &&) const noexcept(false) override final;

    Driver() noexcept(true) = default;

private:
    mutable struct Private_ final {
        struct Event final {
            ::std::type_index key;
            ::std::exception_ptr exception;
        };

        this_::Storage storage = {};
        parent_::parent_::event::Dispatcher<Event> dispatcher = {};
    } private_ = {};

    Driver(Driver &&) = delete;
    Driver(Driver const &) = delete;
    Driver & operator = (Driver &&) = delete;
    Driver & operator = (Driver const &) = delete;
};

struct Pointer final: parent_::pointer::Driver {
    virtual Subscription operator () (
        Handler &&
    ) noexcept(false) override final;

    virtual void * get() const noexcept(true) override final;

    explicit Pointer(void *value = nullptr) noexcept(true);
    virtual ~Pointer() noexcept(true) override final;

private:
    struct Private_ final {
        void *pointer;
        parent_::parent_::event::Dispatcher<void> dispatcher = {};
    } private_;

    Pointer(Pointer &&) = delete;
    Pointer(Pointer const &) = delete;
    Pointer & operator = (Pointer &&) = delete;
    Pointer & operator = (Pointer const &) = delete;
};

} // namespace private_

this_::Type::Type() noexcept(true):
    driver_{new this_::private_::Driver}
{}

this_::Type::Type::~Type() noexcept(true) = default;

namespace lock {
namespace driver {

this_::Type::Type::~Type() noexcept(true) = default;

namespace holder {

this_::Type<void>::Type::~Type() noexcept(true) = default;

this_::Type<void>::Type(void *pointer) noexcept(true): pointer{pointer} {}

} // namespace holder
} // namespace driver
} // namespace lock

namespace driver {

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace driver

namespace pointer {
namespace driver {

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace driver
} // namespace pointer

namespace private_ {
namespace storage {

inline auto invoke_factory(Item &item) noexcept(false) {
    auto const index_ = item.payload.index();

    if (! (
        (2 < index_) && 5 > index_)
    ) throw ::std::invalid_argument{"unknown payload"};

    auto payload_ = ::std::exchange(item.payload, ::std::monostate{});

    try {
        if (
            ::std::exchange(item.pointer, nullptr)
        ) throw ::std::logic_error{"invalid state: pointer not empty"};
        switch(index_) {
        default: break;
        case 3:
            if (
                auto * const pointer_ = ::std::get<3>(payload_)()
            ) item.pointer.reset(new parent_::Pointer{pointer_});
            break;
        case 4:
            if (
                auto holder_ = ::std::get<4>(payload_)()
            ) if (auto * const pointer_ = holder_->pointer) {
                item.pointer.reset(new parent_::Pointer(pointer_));
                item.payload.emplace<2>(::std::move(holder_));
            }
            break;
        }
    }

    catch (...) {
        item.pointer.reset();
        item.payload.emplace<1>(::std::current_exception());
        throw;
    }
}

} // namespace storage

inline void this_::Lock::put(::std::variant<
    void *, ::std::unique_ptr<Holder>,
    ::std::function<void *(void)>,
    ::std::function<::std::unique_ptr<Holder>(void)>
> &&value) noexcept(false) {
    ::std::visit([this] (auto &&value) {
        using Type_ = ::std::decay_t<decltype(value)>;

        if constexpr (::std::is_same_v<::std::unique_ptr<Holder>, Type_>) {
            if (
                ! (value && value->pointer)
            ) throw ::std::invalid_argument{"empty holder"};
        }

        else if constexpr (::std::is_same_v<
            ::std::function<void *(void)>, Type_
        > || ::std::is_same_v<
            ::std::function<::std::unique_ptr<Holder>(void)>, Type_
        >) {
            if (! value) throw ::std::invalid_argument{"empty factory"};
        }

        auto &item_ = [
            sentinel_ = ::std::end(private_.storage),
            iterator_ = private_.storage.find(private_.key)
        ] () -> auto & {
            if (sentinel_ == iterator_) throw ::std::logic_error{
                "key not found in storage"
            };
            return iterator_->second;
        } ();

        if (0 != item_.payload.index()) item_.payload.template emplace<0>();
        if (item_.pointer) item_.pointer.reset();

        try {
            if constexpr (::std::is_same_v<void *, Type_>) {
                if (value) item_.pointer.reset(new this_::Pointer{value});
            }

            else if constexpr (::std::is_same_v<
                ::std::unique_ptr<Holder>, Type_
            >) {
                auto &&pointer_ = ::std::make_shared<
                    this_::Pointer
                >(value->pointer);
                item_.payload.template emplace<2>() = ::std::move(value);
                item_.pointer = ::std::move(pointer_);
            }

            else {
                static_assert(::std::is_same_v<
                    ::std::function<void *(void)>, Type_
                > || ::std::is_same_v<
                    ::std::function<::std::unique_ptr<Holder>(void)>, Type_
                >);
                item_.payload = ::std::move(value);
            }
        }

        catch (...) {
            if (item_.pointer) item_.pointer.reset();
            item_.payload.template emplace<1>(::std::current_exception());
        }
    }, ::std::move(value));
}

inline void this_::Lock::take() noexcept(true) { private_.weak = false; }

inline this_::Lock::Lock(
    ::std::type_index const &key, this_::Storage &storage
) noexcept(true): private_{.key = key, .storage = storage} {}

inline this_::Lock::~Lock() noexcept(true) {
    if (private_.weak) return;
    auto const iterator_ = private_.storage.find(private_.key);
    if (::std::end(private_.storage) == iterator_) return;
    private_.storage.erase(iterator_);
}

inline ::std::shared_ptr<parent_::pointer::Driver> this_::Driver::get(
    ::std::type_index const &key, Qualifiers const &qualifiers
) const noexcept(false) {
    auto &item_ = [
        sentinel_ = ::std::end(private_.storage),
        iterator_ = private_.storage.find(key)
    ] () -> auto & {
        if (sentinel_ == iterator_) throw ::std::invalid_argument{
            "key not found in storage"
        };
        return iterator_->second;
    } ();

    auto const _qualifiers = item_.qualifiers & (~qualifiers);
    if (_qualifiers.any()) throw ::std::invalid_argument{[&_qualifiers] {
        if (! (
            _qualifiers ^ ::std::decay_t<decltype(_qualifiers)>{0x1}
        ).any()) return "unsatisfied const qualifier";
        if (! (
            _qualifiers ^ ::std::decay_t<decltype(_qualifiers)>{0x2}
        ).any()) return "unsatisfied volatile qualifier";
        return "unsatisfied qualifiers: const, volatile";
    } ()};

    auto const index_ = item_.payload.index();

    if (1 == index_) {
        auto const &exception_ = ::std::get<1>(item_.payload);
        if (exception_) ::std::rethrow_exception(exception_);
        throw ::std::logic_error{"invalid state: empty exception"};
    }

    else if (2 < index_) try { this_::storage::invoke_factory(item_); }
    catch (...) {
        private_.dispatcher.dispatch(Private_::Event{
            .key = key, .exception = ::std::current_exception()}
        );
        throw;
    }

    return item_.pointer;
}

inline ::std::unique_ptr<parent_::lock::Driver> this_::Driver::lock(
    ::std::type_index &&key, Qualifiers &&qualifiers
) noexcept(false) {
    auto emplace_result_ = private_.storage.emplace(
        key, this_::storage::Item{.qualifiers = ::std::move(qualifiers)}
    );
    if (! emplace_result_.second) throw ::std::runtime_error{"resource busy"};
    try { return ::std::make_unique<this_::Lock>(
        ::std::move(key), private_.storage
    ); } catch (...) { private_.storage.erase(emplace_result_.first); throw; }
}

inline void this_::Driver::collect() noexcept(true) {
    auto sentinel_ = ::std::end(private_.storage);
    while (true) {
        auto iterator_ = ::std::begin(private_.storage);
        while (true) {
            if (sentinel_ == iterator_) return;
            if (2 < iterator_->second.payload.index()) break;
            ++iterator_;
        }
        try { this_::storage::invoke_factory(iterator_->second); }
        catch (...) { private_.dispatcher.dispatch(Private_::Event{
            .key = iterator_->first,
            .exception = ::std::current_exception()}
        ); }
    }
}

inline parent_::driver::Subscription this_::Driver::on_error(::std::function<
    void(::std::type_index const &, ::std::exception_ptr const &)
> &&handler) const noexcept(false) {
    if (! handler) throw ::std::invalid_argument{"empty handler"};
    return private_.dispatcher.subscribe([
        handler = ::std::move(handler)
    ] (auto &&event) { handler(event.key, event.exception); });
};

inline parent_::driver::Subscription this_::Pointer::operator () (
    Handler &&handler
) noexcept(false) {
    if (! handler) throw ::std::invalid_argument{"empty handler"};
    return private_.dispatcher.subscribe(::std::move(handler));
}

inline void * this_::Pointer::get() const noexcept(true) {
    return private_.pointer;
}

inline this_::Pointer::Pointer(void *value) noexcept(true):
    private_{.pointer = value}
{}

inline this_::Pointer::~Pointer() noexcept(true) {
    if (! private_.pointer) return;
    private_.pointer = nullptr;
    private_.dispatcher.dispatch();
}

} // namespace private_
} // namespace p5::lambda::utils::container
