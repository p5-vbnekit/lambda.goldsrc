#pragma once

#include <cstdarg>
#include <type_traits>

#include "metamod+fwd.hxx"

#include "game.hxx"
#include "engine.hxx"
#include "common.hxx"


namespace p5::lambda::metamod {
namespace api {

inline consteval auto const * version() noexcept(true) { return "5:13"; }
constexpr auto const * version(parent_::Functions const *) noexcept(true);
constexpr auto const * version(parent_::plugin::Functions const *) noexcept(true);

} // namespace api

namespace plugin {

// Information plugin provides about itself.
struct Info final {
    using LoadTime = this_::LoadTime;

    char const *interface_version = parent_::api::version();                // meta_interface version
    char const *name = nullptr;                                             // full name of plugin
    char const *version = nullptr;                                          // version
    char const *date = nullptr;                                             // date
    char const *author = nullptr;                                           // author name/email
    char const *url = nullptr;                                              // URL
    char const *log_tag = nullptr;                                          // log message prefix (unused right now)
    LoadTime loadable = ::std::decay_t<decltype(loadable)>::AnyTime;        // when loadable
    LoadTime unloadable = ::std::decay_t<decltype(unloadable)>::AnyTime;    // when unloadable
};

struct Functions final {
    using GetEntityAPI = common::IntegerBoolean (*)(game::functions::Standard *, int);
    using GetEntityAPI2 = common::IntegerBoolean (*)(game::functions::Standard *, int *);
    using GetNewDllFunctions = common::IntegerBoolean (*)(game::functions::Extension *, int *);
    using GetEngineFunctions = common::IntegerBoolean (*)(engine::Functions *, int *);
    template <class T> struct Pair final { T before = nullptr, after = nullptr; };

    Pair<GetEntityAPI> get_entity_api;
    Pair<GetEntityAPI2> get_entity_api2;
    Pair<GetNewDllFunctions> get_new_dll_functions;
    Pair<GetEngineFunctions> get_engine_functions;
};

} // namespace plugin

struct Globals final {
    plugin::HookResult current = ::std::decay_t<decltype(current)>::Unset;          // writable; plugin's return flag
    plugin::HookResult const previous = ::std::decay_t<decltype(previous)>::Unset;  // readable; return flag of the previous plugin called
    plugin::HookResult const status = ::std::decay_t<decltype(status)>::Unset;      // readable; "highest" return flag so far
    void const *original = nullptr;                                                 // readable; return value from "real" function
    void const *override = nullptr;                                                 // readable; return value from overriding/superceding plugin
};

struct Functions final {
    void (*log_console)(plugin::Info const *, char const *format, ...) = nullptr;
    void (*log_message)(plugin::Info const *, char const *format, ...) = nullptr;
    void (*log_error)(plugin::Info const *, char const *format, ...) = nullptr;
    void (*log_developer)(plugin::Info const *, char const *format, ...) = nullptr;
    void (*center_say)(plugin::Info const *, char const *format, ...) = nullptr;
    void (*center_say_parms)(plugin::Info const *, common::hud::TextParams, char const *format, ...) = nullptr;
    void (*center_say_varargs)(plugin::Info const *, common::hud::TextParams, char const *format, ::std::va_list) = nullptr;
    common::IntegerBoolean (*call_game_entity)(plugin::Info const *, char const *, common::entity::Variables *) = nullptr;
    int (*get_user_msg_id)(plugin::Info const *, char const *name, int *size) = nullptr;
    char const * (*get_user_msg_name)(plugin::Info const *, int id, int *size) = nullptr;
    char const * (*get_plugin_path)(plugin::Info const *) = nullptr;
    char const * (*get_game_info)(plugin::Info const *, GameInfoTag) = nullptr;

    int (*load_plugin)(plugin::Info const *, char const *, plugin::LoadTime, void **handle) = nullptr;
    int (*unload_plugin)(plugin::Info const *, char const *, plugin::LoadTime, plugin::UnloadReason) = nullptr;
    int (*unload_plugin_by_handle)(plugin::Info const *, void *, plugin::LoadTime, plugin::UnloadReason) = nullptr;

    char const *(*is_querying_client_cvar)(plugin::Info const *, common::entity::Dictionary const *player) = nullptr;

    int (*make_request_id)(plugin::Info const *) = nullptr;

    void (*get_hook_tables)(plugin::Info const *, engine::Functions **, game::functions::Standard **, game::functions::Extension **) = nullptr;
};

} // namespace p5::lambda::metamod


#include "metamod+inl.hxx"
