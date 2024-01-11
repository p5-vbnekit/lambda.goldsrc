#include "root_.hxx"
#include "linkspec.h"


P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_EXTERN_C ::p5::lambda::common::IntegerBoolean
P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_EXPORT Meta_Query(
    char const *version,
    ::p5::lambda::metamod::plugin::Info **info,
    ::p5::lambda::metamod::Functions const *functions
) {
    return ::p5::lambda::metamod_plugin::query(version, info, functions);
}

P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_EXTERN_C ::p5::lambda::common::IntegerBoolean
P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_EXPORT Meta_Attach(
    ::p5::lambda::metamod::plugin::LoadTime phase,
    ::p5::lambda::metamod::plugin::Functions *functions,
    ::p5::lambda::metamod::Globals *globals,
    ::p5::lambda::game::functions::Pointers const *game
) {
    return ::p5::lambda::metamod_plugin::attach(
        phase, functions, globals, game
    );
}

P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_EXTERN_C ::p5::lambda::common::IntegerBoolean
P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_EXPORT Meta_Detach(
    ::p5::lambda::metamod::plugin::LoadTime phase,
    ::p5::lambda::metamod::plugin::UnloadReason reason
) {
    return ::p5::lambda::metamod_plugin::detach(phase, reason);
}

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_EXTERN_C void
P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_EXPORT
P5_LAMBDA_METAMOD_PLUGIN_LINKSPEC_WINAPI
GiveFnptrsToDll(
    ::p5::lambda::engine::Functions const *functions,
    ::p5::lambda::engine::Globals *globals
) {
    return ::p5::lambda::metamod_plugin::inject(functions, globals);
}
