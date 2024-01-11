#pragma once

#include <p5/lambda/game.hxx>
#include <p5/lambda/engine.hxx>
#include <p5/lambda/metamod.hxx>

#include "namespace_.hxx"


namespace p5::lambda::metamod_plugin {
namespace api {

namespace parent_ = this_;
namespace this_ = parent_::api;

struct Meta final {
    using Globals = parent_::parent_::metamod::Globals;
    using Functions = parent_::parent_::metamod::Functions;

    struct Plugin final {
        using Info = parent_::parent_::metamod::plugin::Info;
        using Functions = parent_::parent_::metamod::plugin::Functions;

        Info info = {};
        Functions functions = {};
    };

    Plugin plugin = {};
    Functions functions = {};
    Globals *globals = nullptr;
};

struct Game final {
    using Functions = parent_::parent_::game::Functions;

    Functions functions = {};
};

struct Engine final {
    using Globals = parent_::parent_::engine::Globals;
    using Functions = parent_::parent_::engine::Functions;

    Functions functions = {};
    Globals *globals = nullptr;
};

struct Type final {
    using Engine = this_::Engine;
    using Game = this_::Game;
    using Meta = this_::Meta;

    Meta meta = {};
    Game game = {};
    Engine engine = {};
};

Type factory() noexcept(true);

} // namepsace api

using Api = api::Type;

} // namespace p5::lambda::metamod_plugin
