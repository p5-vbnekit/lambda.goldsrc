#include <utility>
#include <variant>
#include <sstream>
#include <optional>
#include <typeindex>
#include <exception>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include <p5/lambda/api.hxx>
#include <p5/lambda/utils/event/dispatcher.hxx>

#include "bindings.hxx"
#include "exception.hxx"
#include "singleton.hxx"


namespace p5::lambda::metamod_plugin::bindings {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

using parent_::parent_::parent_::metamod::plugin::HookResult;

struct Event final {
    ::std::type_index key;
    parent_::Phase phase;
    void const *input;
    void *output;
    ::std::function<void(::std::variant<
        ::std::exception_ptr, this_::HookResult
    >)> response;
};

using Dispatcher = parent_::parent_::parent_::utils::event::Dispatcher<Event>;

inline constexpr static auto transform(
    parent_::Meta<parent_::Phase::Before> value
) noexcept(true) {
    using Result_ = this_::HookResult;
    switch (value) {
    default: break;
    case ::std::decay_t<decltype(value)>::Ignored: return Result_::Ignored;
    case ::std::decay_t<decltype(value)>::Handled: return Result_::Handled;
    case ::std::decay_t<decltype(value)>::Override: return Result_::Override;
    case ::std::decay_t<decltype(value)>::Supercede: return Result_::Supercede;
    }
    return Result_::Unset;
}

inline constexpr static auto transform(
    parent_::Meta<parent_::Phase::After> value
) noexcept(true) {
    using Result_ = this_::HookResult;
    switch (value) {
    default: break;
    case ::std::decay_t<decltype(value)>::Ignored: return Result_::Ignored;
    case ::std::decay_t<decltype(value)>::Handled: return Result_::Handled;
    case ::std::decay_t<decltype(value)>::Override: return Result_::Override;
    }
    return Result_::Unset;
}

template <parent_::Phase phase> inline static auto action_handler(
    ::std::type_index &&key,
    ::std::function<Meta<phase>(void *, void const *)> &&action
) noexcept(true) { return [
    key_ = ::std::move(key), action_ = ::std::move(action)
] (auto const &event) {
    if (key_ != event.key) return;
    if (phase != event.phase) return;
    using State_ = this_::HookResult;
    auto state_ = State_::Unset;
    try { state_ = this_::transform(action_(event.output, event.input)); }
    catch (...) { event.response(::std::current_exception()); }
    if (State_::Unset < state_) event.response(state_);
}; }

namespace target_ = parent_::parent_::binding::target;

using LogMessage_ = ::std::decay_t<decltype(
    parent_::parent_::Singleton::instance().log
)>::Message;

template <parent_::Phase phase, class Target> inline static auto target_handler(
    this_::Dispatcher &dispatcher,
    typename this_::target_::Traits<Target>::Input const &input
) noexcept(true) {
    using Output_ = typename this_::target_::Traits<Target>::Output;
    auto state_ = [] {
        if constexpr (::std::is_void_v<Output_>) {
            struct Result_ final { this_::HookResult meta; };
            return Result_{
                .meta = ::std::decay_t<decltype(Result_::meta)>::Ignored
            };
        }
        else {
            struct Result_ final {
                this_::HookResult meta;
                ::std::optional<Output_> result, temporary;
            };
            return Result_{
                .meta = ::std::decay_t<decltype(Result_::meta)>::Ignored,
                .result = ::std::nullopt, .temporary = ::std::nullopt
            };
        }
    } ();

    auto const &singleton_ = parent_::parent_::Singleton::instance();
    auto const &log_ = singleton_.log;

    auto &&response_ = [&] (auto const &state) {
        ::std::visit([&state_, &log_] (auto const &state) {
            if constexpr (::std::is_same_v<
                ::std::exception_ptr, ::std::decay_t<decltype(state)>
            >) log_.write<LogMessage_::Level::Error>(
                LogMessage_::Location::current()
            ) << parent_::parent_::exception::generate_details(state);
            else {
                static_assert(::std::is_same_v<
                    ::std::decay_t<decltype(state_.meta)>,
                    ::std::decay_t<decltype(state)>
                >);

                if (state_.meta >= state) {
                    if constexpr (requires () {
                        state_.result; state_.temporary;
                    }) if (state_.temporary && (! state_.result)) {
                        state_.result = ::std::exchange(
                            state_.temporary, ::std::nullopt
                        );
                    }
                    return;
                }

                state_.meta = state;

                if constexpr (requires () {
                    state_.result; state_.temporary;
                }) if (state_.temporary) state_.result = ::std::exchange(
                    state_.temporary, ::std::nullopt
                );
            }
        }, state);
    };

    dispatcher.dispatch(this_::Event{
        .key = this_::target_::key<Target>(), .phase = phase,
        .input = &input, .output = [&] () -> void * {
            if constexpr (
                requires () { state_.temporary; }
            ) return &(state_.temporary);
            else return nullptr;
        } (), .response = ::std::move(response_)
    });

    if (auto * const globals_ = singleton_.api.meta.globals) {
        globals_->current = state_.meta;
    }

    if constexpr (! ::std::is_void_v<Output_>) return [
        &value_ = state_.result
    ] {
        if (value_) return *value_;
        return ::std::decay_t<decltype(*value_)>{};
    } ();
}

} // namespace private_

struct Type::Private_ final {
    using Dispatcher = this_::private_::Dispatcher;

    Dispatcher dispatcher = {};

    template <this_::Phase> static
    parent_::parent_::common::IntegerBoolean api(
        parent_::parent_::game::functions::Standard *, int
    ) noexcept(true);

    template <this_::Phase> static
    parent_::parent_::common::IntegerBoolean api(
        parent_::parent_::game::functions::Standard *, int *
    ) noexcept(true);

    template <this_::Phase> static
    parent_::parent_::common::IntegerBoolean api(
        parent_::parent_::game::functions::Extension *, int *
    ) noexcept(true);

    template <this_::Phase> static
    parent_::parent_::common::IntegerBoolean api(
        parent_::parent_::engine::Functions *, int *
    ) noexcept(true);

private:
    template <
        class T,
        class P = typename this_::private_::target_::Traits<T>::Prototype
    > struct Executor_ final { static_assert(::std::is_same_v<
        typename this_::private_::target_::Traits<T>::Prototype, P
    >); };

    static void const * const initializer_;

    static auto const * initialize_() noexcept(true);
};

this_::Injection this_::Type::inject_(::std::type_index &&key, ::std::function<
    Meta<Phase::Before>(void *, void const *)
> &&action) noexcept(false) {
    if (! private_) throw ::std::logic_error{"invalid state"};
    return private_->dispatcher.subscribe(this_::private_::action_handler<
        this_::Phase::Before
    >(::std::move(key), ::std::move(action)));
}

this_::Injection this_::Type::inject_(::std::type_index &&key, ::std::function<
    Meta<Phase::After>(void *, void const *)
> &&action) noexcept(false) {
    return private_->dispatcher.subscribe(this_::private_::action_handler<
        this_::Phase::After
    >(::std::move(key), ::std::move(action)));
}

void this_::Type::apply(Endpoints &endpoints) noexcept(true) {
    endpoints.get_entity_api.before = Private_::api<Phase::Before>;
    endpoints.get_entity_api.after = Private_::api<Phase::After>;
    endpoints.get_entity_api2.before = Private_::api<Phase::Before>;
    endpoints.get_entity_api2.after = Private_::api<Phase::After>;
    endpoints.get_new_dll_functions.before = Private_::api<Phase::Before>;
    endpoints.get_new_dll_functions.after = Private_::api<Phase::After>;
    endpoints.get_engine_functions.before = Private_::api<Phase::Before>;
    endpoints.get_engine_functions.after = Private_::api<Phase::After>;
}

this_::Type::Type() noexcept(true): private_{nullptr} {}
this_::Type::Type::~Type() noexcept(true) = default;

template <class T, class O, class ... I>
struct this_::Type::Private_::Executor_<T, O(I ...)> final {
    template <this_::Phase phase> inline static
    auto execute(I ... input) noexcept(true) {
        using Traits_ = this_::private_::target_::Traits<T>;
        static_assert(::std::is_same_v<typename Traits_::Prototype, O(I ...)>);
        if (auto * const dispatcher_ = [] {
            auto &pointer_ = parent_::Singleton::instance().bindings.private_;
            if (pointer_) return &(pointer_->dispatcher);
            return static_cast<::std::decay_t<
                decltype(pointer_->dispatcher)
            > *>(nullptr);
        } ()) return this_::private_::target_handler<phase, T>(
            *dispatcher_, typename Traits_::Input{input ...}
        );
        if constexpr(! ::std::is_void_v<O>) return typename Traits_::Output{};
    }
};

template <this_::Phase phase> inline
parent_::parent_::common::IntegerBoolean this_::Type::Private_::api(
    parent_::parent_::game::functions::Standard *functions, int version
) noexcept(true) {
    auto const &singleton_ = parent_::Singleton::instance();

    try {
        if (! functions) throw ::std::invalid_argument{
            "empty functions pointer"
        };
        constexpr static auto const version_ = parent_::parent_::api::version<
            ::std::decay_t<decltype(*functions)>
        >();
        if (version_ != version) {
            ::std::ostringstream stream_;
            stream_ << "interface version mismatch: plugin["
            << version_ << "] != game[" << version << "]";
            throw ::std::invalid_argument{stream_.view().data()};
        }
        *functions = {};
        if (! singleton_.bindings.private_) throw ::std::logic_error{
            "invalid state"
        };
    } catch (...) {
        singleton_.log.write<this_::private_::LogMessage_::Level::Error>(
            this_::private_::LogMessage_::Location::current()
        ) << parent_::exception::generate_details();
        return parent_::parent_::common::IntegerBoolean::False;
    }

    functions->gameInit = Executor_<this_::game::Init>::template execute<phase>;
    functions->clientCommand = Executor_<
        this_::game::ClientCommand
    >::template execute<phase>;

    return parent_::parent_::common::IntegerBoolean::True;
}

template <this_::Phase phase> inline
parent_::parent_::common::IntegerBoolean this_::Type::Private_::api(
    parent_::parent_::game::functions::Standard *functions, int *version
) noexcept(true) {
    auto const &log_ = parent_::Singleton::instance().log;

    try {
        if (! functions) throw ::std::invalid_argument{
            "empty functions pointer"
        };
        if (! version) throw ::std::invalid_argument{
            "empty version pointer"
        };
        auto const version_ = ::std::exchange(
            *version, parent_::parent_::api::version(functions)
        );
        if (version_ < *version) {
            ::std::ostringstream stream_;
            stream_ << "interface version mismatch: plugin["
            << *version << "] > game[" << version_ << "]";
            throw ::std::invalid_argument{stream_.view().data()};
        }
    } catch (...) {
        log_.write<this_::private_::LogMessage_::Level::Error>(
            this_::private_::LogMessage_::Location::current()
        ) << parent_::exception::generate_details();
        return parent_::parent_::common::IntegerBoolean::False;
    }

    return api<phase>(functions, *version);
}

template <this_::Phase phase> inline
parent_::parent_::common::IntegerBoolean this_::Type::Private_::api(
    parent_::parent_::game::functions::Extension *functions, int *version
) noexcept(true) {
    auto const &singleton_ = parent_::Singleton::instance();

    try {
        if (! functions) throw ::std::invalid_argument{
            "empty functions pointer"
        };
        if (! version) throw ::std::invalid_argument{
            "empty version pointer"
        };
        auto const version_ = ::std::exchange(
            *version, parent_::parent_::api::version(functions)
        );
        if (version_ < *version) {
            ::std::ostringstream stream_;
            stream_ << "interface version mismatch: plugin["
            << *version << "] > game[" << version_ << "]";
            throw ::std::invalid_argument{stream_.view().data()};
        }
        *functions = {};
        if (! singleton_.bindings.private_) throw ::std::logic_error{
            "invalid state"
        };
    } catch (...) {
        singleton_.log.write<this_::private_::LogMessage_::Level::Error>(
            this_::private_::LogMessage_::Location::current()
        ) << parent_::exception::generate_details();
        return parent_::parent_::common::IntegerBoolean::False;
    }

    functions->gameShutdown = Executor_<
        this_::game::Shutdown
    >::template execute<phase>;

    return parent_::parent_::common::IntegerBoolean::True;
}

template <this_::Phase phase> inline
parent_::parent_::common::IntegerBoolean this_::Type::Private_::api(
    parent_::parent_::engine::Functions *functions, int *version
) noexcept(true) {
    auto const &singleton_ = parent_::Singleton::instance();

    try {
        if (! functions) throw ::std::invalid_argument{
            "empty functions pointer"
        };
        if (! version) throw ::std::invalid_argument{
            "empty version pointer"
        };
        auto const version_ = ::std::exchange(
            *version, parent_::parent_::api::version(functions)
        );
        if (version_ < *version) {
            ::std::ostringstream stream_;
            stream_ << "interface version mismatch: plugin["
            << *version << "] > game[" << version_ << "]";
            throw ::std::invalid_argument{stream_.view().data()};
        }
        *functions = {};
        if (! singleton_.bindings.private_) throw ::std::logic_error{
            "invalid state"
        };
    } catch (...) {
        singleton_.log.write<this_::private_::LogMessage_::Level::Error>(
            this_::private_::LogMessage_::Location::current()
        ) << parent_::exception::generate_details();
        return parent_::parent_::common::IntegerBoolean::False;
    }

    functions->changeLevel = Executor_<
        this_::engine::ChangeLevel
    >::template execute<phase>;

    return parent_::parent_::common::IntegerBoolean::True;
}

inline auto const * this_::Type::Private_::initialize_() noexcept(true) {
    auto &singleton_ = parent_::Singleton::instance();

    try {
        singleton_.bindings.private_.reset(new Private_);
    } catch (...) { singleton_.log.write<
        this_::private_::LogMessage_::Level::Error
    >(
        this_::private_::LogMessage_::Location::current()
    ) << parent_::exception::generate_details(); }

    return static_cast<Type const *>(nullptr);
}

void const * const this_::Type::Private_::initializer_{
    this_::Type::Private_::initialize_()
};

} // namespace p5::lambda::metamod_plugin::bindings
