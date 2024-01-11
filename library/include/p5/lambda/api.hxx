#pragma once

#include "api+fwd.hxx"

#include "game.hxx"
#include "engine.hxx"
#include "metamod.hxx"


namespace p5::lambda::api {

constexpr auto version(parent_::engine::Functions const *) noexcept(true);
constexpr auto version(parent_::game::functions::Standard const *) noexcept(true);
constexpr auto version(parent_::game::functions::Extension const *) noexcept(true);
constexpr auto version(parent_::metamod::Functions const *) noexcept(true);
constexpr auto version(parent_::metamod::plugin::Functions const *) noexcept(true);

} // namespace p5::lambda::api

#include "api+inl.hxx"
