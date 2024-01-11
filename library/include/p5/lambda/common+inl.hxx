#pragma once

#include "common.hxx"


namespace p5::lambda::common {
namespace entity {

inline constexpr auto operator | (EffectFlags l, EffectFlags r) noexcept(true) {
    return static_cast<EffectFlags>(static_cast<int>(l) | static_cast<int>(r));
}

inline constexpr auto operator & (EffectFlags l, EffectFlags r) noexcept(true) {
    return static_cast<EffectFlags>(static_cast<int>(l) & static_cast<int>(r));
}

} // namespace entity
} // namespace p5::lambda::common
