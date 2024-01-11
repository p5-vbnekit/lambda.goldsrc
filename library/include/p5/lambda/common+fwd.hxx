#pragma once

#include <cstdint>

#include "namespace_.hxx"


namespace p5::lambda::common {

namespace parent_ = this_;
namespace this_ = parent_::common;

using Byte = ::std::uint8_t;
using Integer32 = ::std::int32_t;
using UnsignedInteger32 = ::std::uint32_t;
using IntegerString = int;
using Scalar = float;
using Crc32 = unsigned long;

enum class IntegerBoolean: int { False = 0, True = 1 };

enum class AlertType {
    Notice,
    Console,    // same as at_notice, but forces a ConPrintf, not a message box
    AiConsole,  // same as at_console, but only shown if developer level is 2!
    Warning,
    Error,
    Logged      // Server print to console (only in multiplayer games).
};

enum class PrintType { Console, Center, Chat };

enum class ForceType {
    ExactFile,                  // File on client must exactly match server's file
    ModelSameBounds,            // For model files only, the geometry must fit in the same bbox
    ModelSpecifyBounds,         // For model files only, the geometry must fit in the specified bbox
    ModelSpecifyBoundsIfAvail   // For Steam model files only, the geometry must fit in the specified bbox (if the file is available)
};

struct Delta;
struct Customization;
struct TypeDescription;
struct Vector3D;

namespace hud {

namespace parent_ = this_;
namespace this_ = parent_::hud;

struct TextParams;

} // namespace hud

namespace user {

namespace parent_ = this_;
namespace this_ = parent_::user;

struct Command;

} // namespace user

namespace trace {

namespace parent_ = this_;
namespace this_ = parent_::trace;

struct Result;

} // namespace trace

namespace client {

namespace parent_ = this_;
namespace this_ = parent_::client;

struct Data;

} // namespace trace

namespace player {

namespace parent_ = this_;
namespace this_ = parent_::player;

struct Move;

} // namespace player

namespace weapon {

namespace parent_ = this_;
namespace this_ = parent_::weapon;

struct Data;

} // namespace weapon

namespace console {

namespace parent_ = this_;
namespace this_ = parent_::console;

struct Variable;

} // namespace console

namespace network {

namespace parent_ = this_;
namespace this_ = parent_::network;

struct Address;

} // namespace network

namespace sequence {

namespace parent_ = this_;
namespace this_ = parent_::sequence;

struct Entry;

} // namespace sequence

namespace sentence {

namespace parent_ = this_;
namespace this_ = parent_::sentence;

struct Entry;

} // namespace sentence

namespace key_value {

namespace parent_ = this_;
namespace this_ = parent_::key_value;

struct Data;

} // namespace key_value

namespace save_restore {

namespace parent_ = this_;
namespace this_ = parent_::save_restore;

struct Data;

} // namespace save_restore

namespace entity {

namespace parent_ = this_;
namespace this_ = parent_::entity;

enum class RenderMode: int {
    Normal,         // src
    TransColor,     // c*a+dest*(1-a)
    TransTexture,   // src*a+dest*(1-a)
    Glow,           // src*a+dest -- No Z buffer checks
    TransAlpha,     // src*srca+dest*(1-srca)
    TransAdd        // src*a+dest
};

enum class RenderFx: int {
    None = 0,
    PulseSlow,
    PulseFast,
    PulseSlowWide,
    PulseFastWide,
    FadeSlow,
    FadeFast,
    SolidSlow,
    SolidFast,
    StrobeSlow,
    StrobeFast,
    StrobeFaster,
    FlickerSlow,
    FlickerFast,
    NoDissipation,
    Distort,            // Distort/scale/translate flicker
    Hologram,           // Distort + distance fade
    DeadPlayer,         // kRenderAmt is the player index
    Explode,            // Scale up really big!
    GlowShell,          // Glowing Shell
    ClampMinScale,      // Keep this sprite from getting very small (SPRITES only!)
    LightMultiplier     //CTM !!!CZERO added to tell the studiorender that the value in iuser2 is a lightmultiplier
};

enum class MoveType {
    None = 0,           // never moves
    AngleNoClip = 1,
    AngleClip = 2,
    Walk = 3,           // Player only - moving on the ground
    Step = 4,           // gravity, special edge handling -- monsters use this
    Fly = 5,            // No gravity, but still collides with stuff
    Toss = 6,           // gravity/collisions
    Push = 7,           // no clip to world, push and crush
    NoClip = 8,         // No gravity, no collisions, still do velocity/avelocity
    FlyMissile = 9,     // extra size to monsters
    Bounce = 10,        // Just like Toss, but reflect velocity when contacting surfaces
    BounceMissile = 11, // bounce w/o gravity
    Follow = 12,        // track movement of aiment
    PushStep = 13       // BSP model that needs physics/world collisions (uses nearest hull for world collision)
};

// NOTE: Some movetypes will cause collisions independent of SolidState::NOT/SolidState::Trigger when the entity moves
// SOLID only effects OTHER entities colliding with this one when they move - UGH!
enum class SolidType: int {
    Not = 0,        // no interaction with other objects
    Trigger = 1,    // touch on edge, but not blocking
    BBox = 2,       // touch on edge, block
    SlideBox = 3,   // touch on edge, but not an onground
    Bsp = 4         // bsp clip, touch on edge, block
};

enum class DamageState: int { No = 0, Yes = 1, Aim = 2 };

enum class EffectFlags: int {
    None = 0,
    BrightField = 1,    // swirling cloud of particles
    MuzzleFlash = 2,    // single frame ELIGHT on entity attachment 0
    BrightLight = 4,    // DLIGHT centered at entity origin
    DimLight = 8,       // player flashlight
    InvLight = 16,      // get lighting from ceiling
    NoInterp = 32,      // don't interpolate the next frame
    Light = 64,         // rocket flare glow sprite
    NoDraw = 128,       // don't draw entity
    NightVision = 256,  // player nightvision
    SniperLaser = 512,  // sniper laser effect
    FiberCamera = 1024  // fiber camera
};

enum class DeadState: int {
    No = 0,             // alive
    Dying = 1,          // playing death animation or still falling off of a ledge waiting to hit ground
    Dead = 2,           // dead. lying still.
    Respawnable = 3,
    DiscardBody = 4
};

struct Link;
struct State;
struct Variables;
struct Dictionary;

constexpr auto operator | (EffectFlags l, EffectFlags r) noexcept(true);
constexpr auto operator & (EffectFlags l, EffectFlags r) noexcept(true);

} // namespace entity
} // namespace p5::lambda::common


#include "common+inl.hxx"
