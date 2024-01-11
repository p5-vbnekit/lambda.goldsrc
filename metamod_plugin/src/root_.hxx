#pragma once

#include <p5/lambda/game+fwd.hxx>
#include <p5/lambda/engine+fwd.hxx>
#include <p5/lambda/common+fwd.hxx>
#include <p5/lambda/metamod+fwd.hxx>

#include "namespace_.hxx"


namespace p5::lambda::metamod_plugin {

void inject(
    parent_::engine::Functions const *, parent_::engine::Globals *
) noexcept(true);

parent_::common::IntegerBoolean query(
    char const *,
    parent_::metamod::plugin::Info **,
    parent_::metamod::Functions const *
) noexcept(true);

parent_::common::IntegerBoolean attach(
    parent_::metamod::plugin::LoadTime,
    parent_::metamod::plugin::Functions *,
    parent_::metamod::Globals *,
    parent_::game::functions::Pointers const *
) noexcept(true);

parent_::common::IntegerBoolean detach(
    parent_::metamod::plugin::LoadTime, parent_::metamod::plugin::UnloadReason
) noexcept(true);

} // namespace p5::lambda::metamod_plugin
