#pragma once

#include <p5/lambda/engine+fwd.hxx>
#include <p5/lambda/metamod+fwd.hxx>
#include <p5/lambda/utils/log/interface.hxx>

#include "namespace_.hxx"


namespace p5::lambda::metamod_plugin {
namespace log {

namespace parent_ = this_;
namespace this_ = parent_::log;

using Interface = parent_::parent_::utils::log::Interface;

struct Context final {
    struct Meta final {
        using Api = parent_::parent_::metamod::Functions;
        using Plugin = parent_::parent_::metamod::plugin::Info;

        Api const &api;
        Plugin const &plugin;
    };

    using Engine = parent_::parent_::engine::Functions;

    Meta meta;
    Engine const &engine;
};

struct Type final: this_::Interface {
    using Context = this_::Context;
    using Interface = this_::Interface;

    explicit Type(Context &&) noexcept(true);

private:
    Context context_;

    virtual void write_(
        Interface::Message &&
    ) const noexcept(true) override final;

    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace log

using Log = log::Type;

} // namespace p5::lambda::metamod_plugin
