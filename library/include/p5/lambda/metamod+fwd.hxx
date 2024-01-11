#pragma once

#include "namespace_.hxx"


namespace p5::lambda::metamod {

namespace parent_ = this_;
namespace this_ = parent_::metamod;

namespace plugin {

namespace parent_ = this_;
namespace this_ = parent_::plugin;

// Flags for plugin to indicate when it can be be loaded/unloaded.
// NOTE: order is crucial, as greater/less comparisons are made.
enum class LoadTime {
    Never = 0,
    Startup,        // should only be loaded/unloaded at initial hlds execution
    Changelevel,    // can be loaded/unloaded between maps
    AnyTime,        // can be loaded/unloaded at any time
    AnyPause        // can be loaded/unloaded at any time, and can be "paused" during a map
};

// Flags to indicate why the plugin is being unloaded.
enum class UnloadReason {
    Null = 0,
    IniDeleted,     // was deleted from plugins.ini
    FileNewer,      // file on disk is newer than last load
    Command,        // requested by server/console command
    ForcedCommand,  // forced by server/console command
    Delayed,        // delayed from previous request; can't tell origin

    // only used for 'real_reason' on MPlugin::unload()
    Plugin,         // requested by plugin function call
    ForcedPlugin,   // forced by plugin function call

    // only used internally for 'meta reload'
    Reload          // forced unload by reload()
};

enum class HookResult {
    Unset = 0,
    Ignored,    // plugin didn't take any action
    Handled,    // plugin did something, but real function should still be called
    Override,   // call real function, but use my return value
    Supercede   // skip real function; use my return value
};

struct Info;
struct Functions;

} // namespace plugin

enum class GameInfoTag { Name = 0, Description, Directory, ModulePath, ModuleFileName, ModuleRealPath };

struct Globals;
struct Functions;

namespace api {

namespace parent_ = this_;
namespace this_ = parent_::api;

consteval auto const * version() noexcept(true);
consteval void version(void const *) noexcept(true) = delete;
template <class> consteval auto const * version() noexcept(true);

} // namespace api
} // namespace p5::lambda::metamod
